// TIME TOKEN / DEMO CYCLE v3
// Based on the tested FinalACK_180 Token: retain roll detection and 1..8 dots.
// NEW: CHECK <-> clockwise return arrow (both rotated180), synchronized round reboot.
// All online boards must use this package. v3 radio packets are 32 bytes.
// Counts/timer remain RAM-only; the full-left demonstration reboot intentionally clears them.
#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <esp_random.h>
#include <esp_attr.h>
#include <SparkFun_MMA8452Q.h>
#include <cstring>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>


// =====================================================
// PIN SETTINGS
// =====================================================

// XC3732
constexpr uint8_t SDA_PIN = 1;
constexpr uint8_t SCL_PIN = 2;

// MAX7219
constexpr uint8_t DIN_PIN = 4;
constexpr uint8_t CS_PIN  = 5;
constexpr uint8_t CLK_PIN = 6;

// Future vibration
constexpr uint8_t VIBRATION_PIN = 45;


// =====================================================
// TOKEN IDENTITY
// =====================================================

// This Token is currently EXERCISE Token
constexpr uint8_t TOKEN_ID = 1;

enum ActivityType : uint8_t
{
  STUDY = 1,
  EXERCISE = 2,
  GAMING = 3
};

constexpr uint8_t TOKEN_ACTIVITY = EXERCISE;


// =====================================================
// ESP-NOW SETTINGS
// =====================================================

constexpr uint8_t ESPNOW_CHANNEL = 6;


// Broadcast address
uint8_t broadcastAddress[] =
{
  0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF
};



// Protocol v3. All five boards in this package must be updated together.
// Changing roundID invalidates delayed data from a previous demonstration.
// bootID distinguishes a Token-only restart within a round.
constexpr uint32_t DEMO_MAGIC = 0x54544433UL;
constexpr uint8_t DEMO_VERSION = 3;
enum PacketKind : uint8_t { PK_HELLO=1, PK_DATA=2, PK_ACK=3, PK_STATE=4, PK_RESET_READY=5 };
enum DemoPhase : uint8_t { DP_STARTING=0, DP_ACTIVE=1, DP_HOLD=2, DP_FADE=3, DP_RESETTING=4 };
constexpr uint8_t ACK_TOWER_RECEIVED = 1;
constexpr uint8_t ACK_R4_DROP_DONE = 2;
constexpr uint8_t ACK_R4_REJECTED = 3;
constexpr uint8_t ACK_RESULT_UNKNOWN = 4;
struct __attribute__((packed)) DemoPacket {
  uint32_t magic;
  uint8_t version, kind, tokenID, activity;
  uint32_t roundID, bootID, messageNumber;
  uint16_t totalUnits, newUnits;
  uint8_t status;
  uint8_t reserved[3];
  uint32_t argument;
};
static_assert(sizeof(DemoPacket)==32, "v3 packet must be 32 bytes");
struct RadioEvent { DemoPacket p; uint8_t mac[6]; };
DemoPacket makePacket(uint8_t kind);
bool envelopeOK(const DemoPacket &p);
bool sameMessage(const DemoPacket &a,const DemoPacket &b);
bool sameKey(const DemoPacket &a,const DemoPacket &b);
bool deadlineReached(uint32_t now,uint32_t deadline);
inline DemoPacket makePacket(uint8_t kind) {
  DemoPacket p={}; p.magic=DEMO_MAGIC; p.version=DEMO_VERSION; p.kind=kind; return p;
}
inline bool envelopeOK(const DemoPacket &p) {return p.magic==DEMO_MAGIC && p.version==DEMO_VERSION;}
inline bool sameMessage(const DemoPacket &a,const DemoPacket &b) {
  return a.roundID==b.roundID && a.bootID==b.bootID && a.tokenID==b.tokenID
      && a.activity==b.activity && a.messageNumber==b.messageNumber
      && a.totalUnits==b.totalUnits && a.newUnits==b.newUnits;
}
inline bool sameKey(const DemoPacket &a,const DemoPacket &b) {
  return a.roundID==b.roundID && a.bootID==b.bootID && a.tokenID==b.tokenID
      && a.messageNumber==b.messageNumber;
}
inline bool deadlineReached(uint32_t now,uint32_t deadline) {
  return static_cast<int32_t>(now-deadline)>=0;
}

DemoPacket packet={};
uint32_t messageNumber=0, currentRound=0, localBootID=0;
uint8_t towerMac[6]={};bool towerBound=false;
DemoPhase roundPhase=DP_STARTING;
uint32_t lastHelloMs=0;
bool rebootPending=false;
uint32_t tokenRebootAtMs=0, tokenNextRound=0;
// Survives esp_restart only to avoid a repeated REBOOT broadcast rebooting twice.
// This does NOT retain activity counts or the active timer.
struct RebootTicket {uint32_t magic,round,complement;uint8_t mac[6];};
RTC_NOINIT_ATTR RebootTicket rebootTicket;
constexpr uint32_t REBOOT_TICKET_MAGIC=0x52545433UL;

constexpr uint32_t FINAL_ACK_TIMEOUT_MS=35000UL;
constexpr uint32_t RESULT_CUE_MS=2000UL;
constexpr uint32_t TRANSFER_DOT_MS=250UL;
constexpr uint32_t RETURN_SWAP_MS=800UL; // CHECK <-> clockwise arrow, inverted viewing orientation
static_assert(RETURN_SWAP_MS>0,"RETURN_SWAP_MS must be positive");
enum TransferState : uint8_t {TRANSFER_IDLE,TRANSFER_WAITING,TRANSFER_DONE,TRANSFER_REJECTED,TRANSFER_UNKNOWN};
QueueHandle_t ackQueue=nullptr;
TransferState transferState=TRANSFER_IDLE,displayedTransferState=TRANSFER_IDLE;
DemoPacket expectedAckData={};
bool expectedAckValid=false, receiverAckSeen=false;
uint32_t ackWaitStartMs=0,lastTransferDotTick=UINT32_MAX,lastReturnCueTick=UINT32_MAX;
bool resultCueStarted=false;uint32_t resultCueStartMs=0;

void onTransferAck(const esp_now_recv_info_t *info,const uint8_t *data,int len);
void setupTransferAck();
void pollTransferAck();
void setTransferState(TransferState next);
void updateTransferDisplay(uint32_t now);
bool completionCanReturnToReady(uint32_t now);
void handleRoundState(const RadioEvent &event);
void sendResetReady(uint32_t next);
void requestTokenReboot(uint32_t next,uint32_t remain);
bool serviceRound();
void drawReturnArrow(uint8_t frame);
void resetOrientationSamples();
void showWaiting();
void clearMatrix();
void sendTimeUnit();

// =====================================================
// PROTOTYPE SETTINGS
// =====================================================

// Demo duration:
// 8 seconds
constexpr uint32_t DURATION = 8000;


// Roll / normal must remain stable
constexpr uint32_t CONFIRM_MS = 250;


// Current tested orientation values
constexpr float NORMAL_THRESHOLD = 0.60f;
constexpr float FLIPPED_THRESHOLD = -0.30f;


// MAX7219 low brightness
constexpr uint8_t BRIGHTNESS = 0;

// Keep the original sensitivity and orientation sampling values above.
constexpr uint32_t SENSOR_SAMPLE_MS = 20;   // software polls at up to 50 Hz
constexpr uint8_t Z_FILTER_SAMPLES = 3;     // average the last 3 NEW samples
constexpr uint32_t SENSOR_GAP_MS = 150;     // a gap cannot count as a stable hold
constexpr uint32_t FLIP_ATTEMPT_MS = 1500;  // still re-arm the candidate; arrow keeps blinking

// LED-only settings. No new library or wiring is needed.
constexpr uint32_t ARROW_BLINK_MS = 180;    // 180 ms on, 180 ms off; advance one frame per flash
constexpr bool ARROW_MIRROR_X = false;     // true only if the physical display looks counter-clockwise
static_assert(ARROW_BLINK_MS > 0, "ARROW_BLINK_MS must be positive");
constexpr bool DEBUG_ORIENTATION = true;   // false hides periodic diagnostic lines
constexpr uint32_t DEBUG_INTERVAL_MS = 250;



// =====================================================
// SENSOR
// =====================================================

MMA8452Q accel;


// =====================================================
// MAX7219 REGISTERS
// =====================================================

constexpr uint8_t REG_DECODE       = 0x09;
constexpr uint8_t REG_INTENSITY    = 0x0A;
constexpr uint8_t REG_SCAN_LIMIT   = 0x0B;
constexpr uint8_t REG_SHUTDOWN     = 0x0C;
constexpr uint8_t REG_DISPLAYTEST  = 0x0F;


// =====================================================
// HOURGLASS PATTERNS
// =====================================================

const uint8_t HOURGLASS_READY[8] =
{
  0b01111110,
  0b01000010,
  0b00111100,
  0b00011000,
  0b00011000,
  0b00100100,
  0b01000010,
  0b01111110
};


const uint8_t HOURGLASS_BOTTOM[8] =
{
  0b01111110,
  0b01000010,
  0b00100100,
  0b00011000,
  0b00011000,
  0b00111100,
  0b01011010,
  0b01111110
};


// Local orientation guidance only. This arrow NEVER means "Tower received".
// Four genuine clockwise rotations of the same 8x8 curved-arrow bitmap.
// MAX7219 physical row/column mapping may mirror the visible direction;
// use ARROW_MIRROR_X if required, without changing sensor logic.
const uint8_t CLOCKWISE_ARROW_FRAMES[4][8] = {
  { // Frame 0: 0 degrees clockwise
    0b00111100,
    0b01000010,
    0b10000010,
    0b10000111,
    0b10000010,
    0b01000000,
    0b00111000,
    0b00000000
  },
  { // Frame 1: 90 degrees clockwise
    0b00011100,
    0b00100010,
    0b01000001,
    0b01000001,
    0b01000001,
    0b00001001,
    0b00011110,
    0b00001000
  },
  { // Frame 2: 180 degrees clockwise
    0b00000000,
    0b00011100,
    0b00000010,
    0b01000001,
    0b11100001,
    0b01000001,
    0b01000010,
    0b00111100
  },
  { // Frame 3: 270 degrees clockwise
    0b00010000,
    0b01111000,
    0b10010000,
    0b10000010,
    0b10000010,
    0b10000010,
    0b01000100,
    0b00111000
  }
};

// Sparse completion-result icons. They do NOT change the arrow orientation.
// CHECK means only that matching R4 processing completion was reported.
const uint8_t TRANSFER_CHECK[8] = {
  0b00000000,
  0b00000001,
  0b00000010,
  0b00000100,
  0b10001000,
  0b01010000,
  0b00100000,
  0b00000000
};
const uint8_t TRANSFER_REJECT_X[8] = {
  0b10000001,
  0b01000010,
  0b00100100,
  0b00011000,
  0b00011000,
  0b00100100,
  0b01000010,
  0b10000001
};
const uint8_t TRANSFER_UNKNOWN_MARK[8] = {
  0b00011000,
  0b00011000,
  0b00011000,
  0b00011000,
  0b00011000,
  0b00000000,
  0b00011000,
  0b00000000
};

// =====================================================
// TOKEN STATES
// =====================================================

enum TokenState
{
  WAITING_FOR_NORMAL,
  READY,
  CONFIRMING_FLIP,
  RUNNING,
  COMPLETED
};

TokenState state = WAITING_FOR_NORMAL;


// =====================================================
// RUNTIME DATA
// =====================================================

// RAM only. Preserved legacy meaning: completed total since this boot.
// This version neither clears this number nor claims it is an ACK-backed
// count of untransferred units. packet.totalUnits still uses this value.
uint16_t pendingUnits = 0;


uint32_t sessionStartTime = 0;

uint32_t orientationStartTime = 0;

bool orientationTiming = false;

uint8_t currentStage = 0;

// Flip-only helper data; no new main states or new hardware.
float zHistory[Z_FILTER_SAMPLES] = {};
float filteredZ = 0.0f;
uint8_t zHistoryCount = 0;
uint8_t zHistoryNext = 0;
uint32_t lastSensorPollTime = 0;
uint32_t lastGoodSampleTime = 0;
bool haveSensorSample = false;
uint32_t flipAttemptStartTime = 0;
// Display guidance is independent of candidate timeout/retry state.
bool arrowGuideActive = false;
uint32_t arrowGuideStartTime = 0;
uint32_t lastArrowTick = UINT32_MAX;
uint8_t currentArrowFrame = 0;
uint32_t lastDebugTime = 0;


// =====================================================
// MAX7219 BASIC FUNCTIONS
// =====================================================

void maxWrite(
  uint8_t reg,
  uint8_t data
)
{
  digitalWrite(
    CS_PIN,
    LOW
  );

  delayMicroseconds(2);


  shiftOut(
    DIN_PIN,
    CLK_PIN,
    MSBFIRST,
    reg
  );


  shiftOut(
    DIN_PIN,
    CLK_PIN,
    MSBFIRST,
    data
  );


  delayMicroseconds(2);


  digitalWrite(
    CS_PIN,
    HIGH
  );


  delayMicroseconds(2);
}


// -----------------------------------------------------
// Init Matrix
// -----------------------------------------------------

void initMatrix()
{
  maxWrite(
    REG_DISPLAYTEST,
    0x00
  );

  maxWrite(
    REG_DECODE,
    0x00
  );

  maxWrite(
    REG_SCAN_LIMIT,
    0x07
  );

  maxWrite(
    REG_INTENSITY,
    BRIGHTNESS
  );

  maxWrite(
    REG_SHUTDOWN,
    0x01
  );
}


// -----------------------------------------------------
// Clear
// -----------------------------------------------------

void clearMatrix()
{
  for (
    uint8_t row = 1;
    row <= 8;
    row++
  )
  {
    maxWrite(
      row,
      0x00
    );
  }
}


// -----------------------------------------------------
// Bitmap
// -----------------------------------------------------

void showPattern(
  const uint8_t pattern[8]
)
{
  for (
    uint8_t row = 1;
    row <= 8;
    row++
  )
  {
    maxWrite(
      row,
      pattern[row - 1]
    );
  }
}


// Rotate only the requested transfer cues: reverse row order AND bit order.
// Do not change showPattern() globally: arrow/hourglass/progress stay unchanged.
void showPatternRotated180(const uint8_t pattern[8])
{
  uint8_t rotated[8] = {};
  for (uint8_t row = 0; row < 8; ++row)
  {
    for (uint8_t bit = 0; bit < 8; ++bit)
    {
      if (pattern[row] & (1U << bit))
      {
        rotated[7U - row] |= static_cast<uint8_t>(1U << (7U - bit));
      }
    }
  }
  showPattern(rotated);
}

// =====================================================
// NON-BLOCKING CLOCKWISE ARROW CUE
// =====================================================

void drawArrowFrame(uint8_t frame)
{
  const uint8_t* source = CLOCKWISE_ARROW_FRAMES[frame % 4];
  if (!ARROW_MIRROR_X)
  {
    showPattern(source);
    return;
  }

  // Mirror only the new arrow cue; leave hourglass and progress unchanged.
  uint8_t mirrored[8] = {};
  for (uint8_t row = 0; row < 8; ++row)
  {
    for (uint8_t col = 0; col < 8; ++col)
    {
      if (source[row] & (1U << col))
      {
        mirrored[row] |= static_cast<uint8_t>(1U << (7 - col));
      }
    }
  }
  showPattern(mirrored);
}

void updateArrowGuide(uint32_t now)
{
  if (!arrowGuideActive || (state != READY && state != CONFIRMING_FLIP))
  {
    return;
  }

  const uint32_t tick = (now - arrowGuideStartTime) / ARROW_BLINK_MS;
  if (tick == lastArrowTick)
  {
    return; // Send a new bitmap only when the visible frame changes.
  }
  lastArrowTick = tick;

  if ((tick % 2U) == 0U)
  {
    currentArrowFrame = static_cast<uint8_t>((tick / 2U) % 4U);
    drawArrowFrame(currentArrowFrame);
  }
  else
  {
    clearMatrix();
  }
}

void beginArrowGuide(uint32_t now)
{
  if (arrowGuideActive)
  {
    return; // Do not restart the animation after a brief wobble or timeout.
  }
  arrowGuideActive = true;
  arrowGuideStartTime = now;
  lastArrowTick = UINT32_MAX;
  currentArrowFrame = 0;
  updateArrowGuide(now); // First frame appears immediately, without delay().
}

// =====================================================
// LED WAITING
// =====================================================

void showWaiting()
{
  clearMatrix();

  maxWrite(
    1,
    0b10000000
  );

  maxWrite(
    8,
    0b00000001
  );
}


// =====================================================
// LED READY
// =====================================================

void showReady()
{
  initMatrix();

  showPattern(
    HOURGLASS_READY
  );
}


// =====================================================
// LED PROGRESS
// =====================================================

void showProgress(
  uint8_t stage
)
{
  if (stage > 8)
  {
    stage = 8;
  }

  clearMatrix();

  // 8-second demo: one FULL 8-pixel row per stage, rotated 180 degrees for flipped viewing.
  // Stage 1 = physical TOP row. In the flipped viewing position it is the BOTTOM row.
  // Stage 8 = all 64 pixels. Other displays are NOT rotated here.
  for (uint8_t i = 0; i < stage; ++i)
  {
    maxWrite(1 + i, 0xFF);
  }
}


// =====================================================
// COMPLETE FEEDBACK
// =====================================================

// Keep the previous completion indicator: eight dots on the bottom row.
// This is separate from the NEW full-panel timer progress.
void showCompletionDots()
{
  clearMatrix();
  maxWrite(8, 0xFF);
}

void showCompleteFeedback()
{
  // Stage 8 briefly shows the completely filled matrix at 8 seconds.
  showProgress(8);

  delay(200);

  clearMatrix();

  delay(150);

  // Afterwards keep the original simple 8-dot completion indication.
  showCompletionDots();

  delay(200);

  clearMatrix();

  delay(150);

  showCompletionDots();
}


// =====================================================
// HOURGLASS RESET
// =====================================================

void showHourglassReset()
{
  showPattern(
    HOURGLASS_BOTTOM
  );

  delay(400);


  showPattern(
    HOURGLASS_READY
  );
}


// =====================================================
// ORIENTATION
// =====================================================

bool isNormal()
{
  return zHistoryCount == Z_FILTER_SAMPLES
      && filteredZ > NORMAL_THRESHOLD
      && accel.cz > 0.0f;
}


bool isFlipped()
{
  // Raw sign check prevents the old filtered value from starting a session
  // on a sample that is already facing upward again.
  return zHistoryCount == Z_FILTER_SAMPLES
      && filteredZ < FLIPPED_THRESHOLD
      && accel.cz < 0.0f;
}


// =====================================================
// ORIENTATION HELPERS -- RETAINED FLIP FIX, UPDATED LED CUE
// =====================================================

void resetOrientationSamples()
{
  zHistoryCount = 0;
  zHistoryNext = 0;
  filteredZ = 0.0f;
  haveSensorSample = false;
  orientationTiming = false;
}

bool readOrientationSample(uint32_t now)
{
  // A missing sample must not silently count toward the 250 ms hold.
  if (haveSensorSample && now - lastGoodSampleTime > SENSOR_GAP_MS)
  {
    resetOrientationSamples();
    Serial.println("SENSOR_GAP: hold restarted; waiting for fresh readings.");
  }

  if (now - lastSensorPollTime < SENSOR_SAMPLE_MS)
  {
    return false;
  }
  lastSensorPollTime = now;

  if (!accel.available())
  {
    return false;
  }

  accel.read();
  lastGoodSampleTime = millis();
  haveSensorSample = true;

  zHistory[zHistoryNext] = accel.cz;
  zHistoryNext = (zHistoryNext + 1) % Z_FILTER_SAMPLES;
  if (zHistoryCount < Z_FILTER_SAMPLES)
  {
    ++zHistoryCount;
  }

  float sum = 0.0f;
  for (uint8_t i = 0; i < zHistoryCount; ++i)
  {
    sum += zHistory[i];
  }
  filteredZ = sum / zHistoryCount;
  return true;
}

bool normalHeldLongEnough(uint32_t now)
{
  if (!isNormal())
  {
    orientationTiming = false;
    return false;
  }
  if (!orientationTiming)
  {
    orientationTiming = true;
    orientationStartTime = now;
  }
  return now - orientationStartTime >= CONFIRM_MS;
}

void beginFlipAttempt(uint32_t now)
{
  state = CONFIRMING_FLIP;
  flipAttemptStartTime = now;
  orientationStartTime = now;
  orientationTiming = true;
  beginArrowGuide(now);
  Serial.println("FLIP_CANDIDATE: keep turning the top downward and hold.");
}

void returnReadyForRetry(uint32_t now)
{
  // Keep the existing retry logic: stay armed, never force WAITING_FOR_NORMAL.
  state = READY;
  orientationTiming = false;
  // The prompt must continue until success, not be interrupted by X/hourglass.
  beginArrowGuide(now);
  Serial.println("NOT_STARTED: retry allowed; clockwise arrow continues.");
}



const char* tokenStateName()
{
  switch (state)
  {
    case WAITING_FOR_NORMAL: return "WAITING_FOR_NORMAL";
    case READY: return "READY";
    case CONFIRMING_FLIP: return "CONFIRMING_FLIP";
    case RUNNING: return "RUNNING";
    case COMPLETED: return "COMPLETED";
  }
  return "UNKNOWN";
}

void printOrientationDebug(uint32_t now)
{
  if (!DEBUG_ORIENTATION || now - lastDebugTime < DEBUG_INTERVAL_MS)
  {
    return;
  }
  lastDebugTime = now;
  Serial.print("DIAG state=");
  Serial.print(tokenStateName());
  Serial.print(" fresh=");
  Serial.print(haveSensorSample ? "yes" : "no");
  Serial.print(" x="); Serial.print(accel.cx, 2);
  Serial.print(" y="); Serial.print(accel.cy, 2);
  Serial.print(" z="); Serial.print(accel.cz, 2);
  Serial.print(" zAvg="); Serial.print(filteredZ, 2);
  Serial.print(" holdMs=");
  Serial.println(orientationTiming ? now - orientationStartTime : 0);
}

// =====================================================
// END-TO-END RESULT: callback queues; loop validates and displays.
// A receive-only ACK must never finish the transfer or show a check mark.
// =====================================================


// Reuse the already-tested arrow, but rotate only the completion prompt by180.
void drawReturnArrow(uint8_t frame) {
  const uint8_t *src=CLOCKWISE_ARROW_FRAMES[frame%4U];uint8_t image[8]={};
  for(uint8_t row=0;row<8;++row)for(uint8_t bit=0;bit<8;++bit)
    if(src[row]&(1U<<bit))image[row]|=uint8_t(1U<<(ARROW_MIRROR_X?7U-bit:bit));
  showPatternRotated180(image);
}
void sendResetReady(uint32_t next) {
  if(!towerBound)return;
  DemoPacket p=makePacket(PK_RESET_READY);p.tokenID=TOKEN_ID;p.activity=TOKEN_ACTIVITY;
  p.bootID=localBootID;p.roundID=next;
  esp_now_send(towerMac,reinterpret_cast<const uint8_t*>(&p),sizeof(p));
}
void requestTokenReboot(uint32_t next,uint32_t remain) {
  if(remain>10000UL)return;
  bool already=rebootTicket.magic==REBOOT_TICKET_MAGIC && rebootTicket.round==next
      && rebootTicket.complement==~next && memcmp(rebootTicket.mac,towerMac,6)==0;
  sendResetReady(next);
  if(already){currentRound=next;roundPhase=DP_RESETTING;return;}
  const uint32_t candidate=millis()+remain;
  if(!rebootPending || deadlineReached(tokenRebootAtMs,candidate))tokenRebootAtMs=candidate;
  if(!rebootPending)Serial.printf("DEMO REBOOT SCHEDULED: new round=%lu, remaining=%lu ms\n",(unsigned long)next,(unsigned long)remain);
  rebootPending=true;tokenNextRound=next;roundPhase=DP_RESETTING;orientationTiming=false;
}
void handleRoundState(const RadioEvent &e) {
  const DemoPacket &p=e.p;
  if(p.roundID==0||p.status>DP_RESETTING)return;
  if(towerBound && memcmp(towerMac,e.mac,6)!=0)return; // one demo Tower, not authentication
  if(!towerBound) {
    memcpy(towerMac,e.mac,6);towerBound=true;
    if(!esp_now_is_peer_exist(towerMac)) {
      esp_now_peer_info_t peer={};memcpy(peer.peer_addr,towerMac,6);
      peer.channel=ESPNOW_CHANNEL;peer.ifidx=WIFI_IF_STA;peer.encrypt=false;
      esp_err_t r=esp_now_add_peer(&peer);
      if(r!=ESP_OK&&r!=ESP_ERR_ESPNOW_EXIST){towerBound=false;return;}
    }
  }
  bool validTicket=rebootTicket.magic==REBOOT_TICKET_MAGIC && rebootTicket.complement==~rebootTicket.round
      && memcmp(rebootTicket.mac,towerMac,6)==0;
  if(validTicket && p.roundID<rebootTicket.round)return; // buffered older control after our reboot
  if(rebootPending) {
    if(p.status==DP_RESETTING&&p.roundID==tokenNextRound)requestTokenReboot(p.roundID,p.argument);
    return;
  }
  if(p.status==DP_RESETTING) {requestTokenReboot(p.roundID,p.argument);return;}
  if(currentRound!=0 && p.roundID!=currentRound) {
    if(p.roundID<currentRound)return;
    Serial.println("NEW ROUND BEACON: rebooting to discard previous demo RAM.");
    requestTokenReboot(p.roundID,100);return;
  }
  bool freshRound=currentRound==0;
  DemoPhase old=roundPhase;currentRound=p.roundID;roundPhase=DemoPhase(p.status);
  if((freshRound||old!=DP_ACTIVE)&&roundPhase==DP_ACTIVE) {
    resetOrientationSamples();
    if(state!=COMPLETED){state=WAITING_FOR_NORMAL;arrowGuideActive=false;showWaiting();}
    Serial.printf("ROUND READY %lu. Place NORMAL to arm.\n",(unsigned long)currentRound);
  }
  if((roundPhase==DP_HOLD||roundPhase==DP_FADE)&&old!=roundPhase) {
    orientationTiming=false;arrowGuideActive=false;
    // Pause new work immediately. An unfinished session is intentionally discarded by the upcoming reboot.
    if(state!=COMPLETED)showWaiting();
    Serial.println("DEMO ROUND CLOSING. New sessions disabled; incomplete timer will be discarded at reboot.");
  }
}
bool serviceRound() {
  uint32_t now=millis();
  if(now-lastHelloMs>=2000UL) {
    lastHelloMs=now;DemoPacket hello=makePacket(PK_HELLO);
    hello.tokenID=TOKEN_ID;hello.activity=TOKEN_ACTIVITY;hello.bootID=localBootID;hello.roundID=currentRound;
    esp_now_send(towerBound?towerMac:broadcastAddress,reinterpret_cast<const uint8_t*>(&hello),sizeof(hello));
  }
  if(rebootPending && deadlineReached(now,tokenRebootAtMs)) {
    rebootTicket.magic=REBOOT_TICKET_MAGIC;rebootTicket.round=tokenNextRound;rebootTicket.complement=~tokenNextRound;
    memcpy(rebootTicket.mac,towerMac,6);
    clearMatrix();Serial.println("TOKEN SOFTWARE REBOOT NOW; demo counters/timer reset.");delay(30);esp_restart();
  }
  return rebootPending || !towerBound || currentRound==0 || roundPhase!=DP_ACTIVE;
}

void onTransferAck(const esp_now_recv_info_t *info,const uint8_t *data,int len)
{
  if(!ackQueue||!info||!data||len!=int(sizeof(DemoPacket)))return;
  RadioEvent e={};memcpy(&e.p,data,sizeof(e.p));memcpy(e.mac,info->src_addr,6);
  if(!envelopeOK(e.p))return;
  if(e.p.kind!=PK_STATE && e.p.kind!=PK_ACK)return;
  if(e.p.kind==PK_ACK && (e.p.tokenID!=TOKEN_ID||e.p.activity!=TOKEN_ACTIVITY))return;
  xQueueSend(ackQueue,&e,0);
}

void setupTransferAck()
{
  localBootID=esp_random();if(localBootID==0)localBootID=1;
  ackQueue=xQueueCreate(16,sizeof(RadioEvent));
  if(!ackQueue){while(true){Serial.println("ERROR: radio queue allocation failed");delay(2000);}}
  if(esp_now_register_recv_cb(onTransferAck)!=ESP_OK){while(true){Serial.println("ERROR: callback registration failed");delay(2000);}}
  Serial.printf("TOKEN DEMO CYCLE v3: id=%u activity=%u boot=%lu\n",unsigned(TOKEN_ID),unsigned(TOKEN_ACTIVITY),(unsigned long)localBootID);
  Serial.println("Waiting for Tower round. Final R4 result required for CHECK.");
  lastHelloMs=millis()-2000UL;
}

void setTransferState(TransferState next)
{
  if(transferState==next)return;
  transferState=next;resultCueStarted=false;lastTransferDotTick=UINT32_MAX;lastReturnCueTick=UINT32_MAX;
}

void pollTransferAck()
{
  if(!ackQueue)return;
  RadioEvent event;
  while(xQueueReceive(ackQueue,&event,0)==pdTRUE) {
    if(event.p.kind==PK_STATE){handleRoundState(event);continue;}
    const DemoPacket &a=event.p;
    if(!towerBound || memcmp(towerMac,event.mac,6)!=0 || !expectedAckValid
       || a.roundID!=currentRound || a.bootID!=localBootID || !sameMessage(a,expectedAckData))continue;
    if(a.status<ACK_TOWER_RECEIVED||a.status>ACK_RESULT_UNKNOWN)continue;
    if(a.status==ACK_TOWER_RECEIVED) {
      if(!receiverAckSeen){receiverAckSeen=true;Serial.println("RECEIVER ACK: Tower radio received it; still waiting for R4.");}
      continue;
    }
    if(transferState==TRANSFER_DONE||transferState==TRANSFER_REJECTED)continue;
    if(a.status==ACK_R4_DROP_DONE) {
      setTransferState(TRANSFER_DONE);
      Serial.println("=== FINAL DROP ACK CONFIRMED ===");
      Serial.printf("Token=%u activity=%u round=%lu boot=%lu message=%lu\n",unsigned(TOKEN_ID),unsigned(TOKEN_ACTIVITY),
        (unsigned long)a.roundID,(unsigned long)a.bootID,(unsigned long)a.messageNumber);
      Serial.println("R4 software completed scene update; not optical or permanent-storage proof.");
      Serial.println("CHECK <-> CLOCKWISE ARROW: turn Token back for the next session.");
    } else if(a.status==ACK_R4_REJECTED) {
      setTransferState(TRANSFER_REJECTED);Serial.println("FINAL REJECTED: not added. See Tower for reason.");
    } else {setTransferState(TRANSFER_UNKNOWN);Serial.println("FINAL UNKNOWN: no success claimed, no automatic resend.");}
    Serial.print("pendingUnits unchanged until demo reboot = ");Serial.println(pendingUnits);
  }
  if(expectedAckValid && transferState==TRANSFER_WAITING && millis()-ackWaitStartMs>=FINAL_ACK_TIMEOUT_MS) {
    setTransferState(TRANSFER_UNKNOWN);Serial.println("FINAL ACK TIMEOUT: UNKNOWN, not proof of data loss.");
  }
}

void updateTransferDisplay(uint32_t now)
{
  if(state!=COMPLETED||transferState==TRANSFER_IDLE)return;
  if(displayedTransferState!=transferState) {
    displayedTransferState=transferState;lastTransferDotTick=UINT32_MAX;lastReturnCueTick=UINT32_MAX;initMatrix();
  }
  if(transferState==TRANSFER_WAITING) {
    const uint32_t tick=(now-ackWaitStartMs)/TRANSFER_DOT_MS;
    if(tick!=lastTransferDotTick) {
      lastTransferDotTick=tick;uint8_t image[8]={};uint8_t count=uint8_t((tick%4U+1U)%4U);
      for(uint8_t i=0;i<count;++i)image[3]|=uint8_t(1U<<(5U-i));
      showPatternRotated180(image);
    } return;
  }
  if(!resultCueStarted){resultCueStarted=true;resultCueStartMs=now;lastReturnCueTick=UINT32_MAX;}
  if(transferState==TRANSFER_DONE) {
    const uint32_t tick=(now-resultCueStartMs)/RETURN_SWAP_MS;
    if(tick!=lastReturnCueTick) {
      lastReturnCueTick=tick;
      if((tick%2U)==0)showPatternRotated180(TRANSFER_CHECK);
      else drawReturnArrow(uint8_t((tick/2U)%4U));
    }
  } else if(lastReturnCueTick==UINT32_MAX) {
    lastReturnCueTick=0;
    if(transferState==TRANSFER_REJECTED)showPattern(TRANSFER_REJECT_X);
    else showPattern(TRANSFER_UNKNOWN_MARK);
  }
}

bool completionCanReturnToReady(uint32_t now)
{
  return transferState != TRANSFER_WAITING && transferState != TRANSFER_IDLE
      && resultCueStarted && static_cast<uint32_t>(now - resultCueStartMs) >= RESULT_CUE_MS;
}

// =====================================================
// SEND ONE NEW UNIT -- original unit counts, plus v3 round and boot IDs.
// One in-flight session at a time; the completion screen waits for a result.
// =====================================================
void sendTimeUnit()
{
  ++messageNumber;packet=makePacket(PK_DATA);
  packet.tokenID=TOKEN_ID;packet.activity=TOKEN_ACTIVITY;
  packet.roundID=currentRound;packet.bootID=localBootID;
  packet.totalUnits=pendingUnits;packet.newUnits=1;packet.messageNumber=messageNumber;
  expectedAckData=packet;expectedAckValid=true;receiverAckSeen=false;ackWaitStartMs=millis();
  displayedTransferState=TRANSFER_IDLE;setTransferState(TRANSFER_WAITING);
  if(!towerBound||roundPhase!=DP_ACTIVE||rebootPending){setTransferState(TRANSFER_UNKNOWN);return;}
  const esp_err_t r=esp_now_send(towerMac,reinterpret_cast<const uint8_t*>(&packet),sizeof(packet));
  Serial.printf("Sending id=%u activity=%u units=1 total=%u msg=%lu round=%lu\n",unsigned(TOKEN_ID),unsigned(TOKEN_ACTIVITY),
    unsigned(pendingUnits),(unsigned long)messageNumber,(unsigned long)currentRound);
  if(r!=ESP_OK){setTransferState(TRANSFER_UNKNOWN);Serial.println("Send queue failed; no success claimed.");}
}


// =====================================================
// ENTER WAITING
// =====================================================

void enterWaiting()
{
  state = WAITING_FOR_NORMAL;
  arrowGuideActive = false;
  lastArrowTick = UINT32_MAX;
  resetOrientationSamples();
  showWaiting();
  Serial.println();
  Serial.println("STATE: WAITING_FOR_NORMAL");
}


// =====================================================
// ENTER READY
// =====================================================

void enterReady(
  bool playResetAnimation
)
{
  arrowGuideActive = false;
  lastArrowTick = UINT32_MAX;

  if (playResetAnimation)
  {
    Serial.println(
      "Hourglass reset..."
    );

    showHourglassReset();
  }
  else
  {
    showReady();
  }


  state =
    READY;


  orientationTiming =
    false;


  currentStage =
    0;


  Serial.println();
  Serial.println(
    "STATE: READY"
  );


  Serial.print(
    "pendingUnits = "
  );

  Serial.println(
    pendingUnits
  );


  Serial.println(
    "Roll Token over to start."
  );
}


// =====================================================
// START SESSION
// =====================================================

void startSession()
{
  // Previous result belongs to the previous session. A stale ACK must not
  // paint a check mark over the next running session.
  expectedAckValid = false;
  transferState = TRANSFER_IDLE;
  displayedTransferState = TRANSFER_IDLE;
  resultCueStarted = false;
  state = RUNNING;
  sessionStartTime = millis();
  currentStage = 0;
  orientationTiming = false;
  arrowGuideActive = false;
  lastArrowTick = UINT32_MAX;

  // Freeze the last arrow visibly, even if confirmation happened in an OFF
  // frame. The timer starts NOW; no delay and no prematurely earned dot.
  // With DURATION=8000, the first progress dot replaces it after 1000 ms.
  drawArrowFrame(currentArrowFrame);

  Serial.println();
  Serial.println(">>> ROLL CONFIRMED");
  Serial.println(">>> SESSION STARTED");
  Serial.println("STATE: RUNNING");
}


// =====================================================
// COMPLETE SESSION
// =====================================================

void completeSession()
{
  if (state != RUNNING) return;

  // Prevent duplicate completion
  state =
    COMPLETED;


  currentStage =
    8;


  // -------------------------------------------------
  // Accumulate inside Token
  // -------------------------------------------------

  pendingUnits++;


  Serial.println();
  Serial.println(
    "=========================="
  );

  Serial.println(
    ">>> SESSION COMPLETE"
  );


  Serial.println(
    ">>> +1 SIMULATED TIME UNIT"
  );


  Serial.print(
    ">>> pendingUnits = "
  );

  Serial.println(
    pendingUnits
  );


  Serial.println(
    "=========================="
  );


  // -------------------------------------------------
  // Send this new Time Unit
  // -------------------------------------------------

  sendTimeUnit();


  // -------------------------------------------------
  // Local completion feedback
  // -------------------------------------------------

  showCompleteFeedback();


  resetOrientationSamples();
  updateTransferDisplay(millis());


  Serial.println(
    "STATE: COMPLETED"
  );


  Serial.println(
    "Wait for the final result; return to NORMAL after the result cue."
  );
}


// =====================================================
// UPDATE SESSION
// =====================================================

void updateSession()
{
  uint32_t elapsed = millis() - sessionStartTime;

  if (elapsed >= DURATION)
  {
    completeSession();
    return;
  }

  uint8_t newStage = (elapsed * 8UL) / DURATION;

  if (newStage == 0)
  {
    // Keep the arrow still until the first original progress threshold.
    // Do not blink/rotate, add a play icon, or change the earned-dot timing.
    return;
  }

  if (newStage != currentStage)
  {
    currentStage = newStage;
    showProgress(currentStage);
    Serial.print("Progress: ");
    Serial.print(currentStage);
    Serial.println(" / 8");
  }
}


// =====================================================
// SETUP
// =====================================================

void setup()
{
  Serial.begin(
    115200
  );


  delay(
    1500
  );


  Serial.println();
  Serial.println(
    "================================"
  );

  Serial.println(
    "TIME TOKEN EXERCISE | DEMO CYCLE v3"
  );

  Serial.println(
    "================================"
  );


  // =================================================
  // MAX7219
  // =================================================

  pinMode(
    DIN_PIN,
    OUTPUT
  );

  pinMode(
    CLK_PIN,
    OUTPUT
  );

  pinMode(
    CS_PIN,
    OUTPUT
  );


  digitalWrite(
    CS_PIN,
    HIGH
  );

  digitalWrite(
    CLK_PIN,
    LOW
  );

  digitalWrite(
    DIN_PIN,
    LOW
  );


  initMatrix();

  clearMatrix();


  // =================================================
  // XC3732
  // =================================================

  Wire.begin(
    SDA_PIN,
    SCL_PIN
  );


  if (
    !accel.begin()
  )
  {
    Serial.println(
      "ERROR: XC3732 not detected."
    );


    while (true)
    {
      delay(1000);
    }
  }


  Serial.println(
    "XC3732 detected."
  );


  // =================================================
  // ESP-NOW
  // =================================================

  WiFi.mode(
    WIFI_STA
  );


  delay(
    100
  );


  esp_wifi_set_channel(
    ESPNOW_CHANNEL,
    WIFI_SECOND_CHAN_NONE
  );


  Serial.print(
    "Token MAC: "
  );

  Serial.println(
    WiFi.macAddress()
  );


  Serial.print(
    "ESP-NOW Channel: "
  );

  Serial.println(
    ESPNOW_CHANNEL
  );


  if (
    esp_now_init()
    != ESP_OK
  )
  {
    Serial.println(
      "ERROR: ESP-NOW init failed."
    );


    while (true)
    {
      delay(1000);
    }
  }


  // -------------------------------------------------
  // Add broadcast peer
  // -------------------------------------------------

  esp_now_peer_info_t peer = {};


  memcpy(
    peer.peer_addr,
    broadcastAddress,
    6
  );


  peer.channel =
    ESPNOW_CHANNEL;


  peer.encrypt =
    false;


  peer.ifidx =
    WIFI_IF_STA;


  if (
    esp_now_add_peer(&peer)
    != ESP_OK
  )
  {
    Serial.println(
      "ERROR: Failed to add ESP-NOW peer."
    );


    while (true)
    {
      delay(1000);
    }
  }


  Serial.println(
    "ESP-NOW ready."
  );

  // Register v3 radio: includes round and boot IDs; all boards must be updated together.
  setupTransferAck();


  // =================================================
  // START
  // =================================================

  Serial.println();

  Serial.print(
    "Token ID: "
  );

  Serial.println(
    TOKEN_ID
  );


  Serial.println(
    "Activity: EXERCISE"
  );


  Serial.print(
    "Demo duration: "
  );

  Serial.print(
    DURATION / 1000
  );

  Serial.println(
    " seconds"
  );


  Serial.println(
    "pendingUnits = 0"
  );


  enterWaiting();
}


// =====================================================
// MAIN LOOP
// =====================================================

void loop()
{
  // Service radio regardless of sensor data; the callback never renders LED.
  pollTransferAck();
  const bool roundBlocked=serviceRound();
  updateTransferDisplay(millis());
  if(roundBlocked){delay(10);return;}

  // Preserve original behavior: once started, movement never cancels,
  // restarts or pauses the session; sensor polling cannot gate this timer.
  if (state == RUNNING)
  {
    updateSession();
    delay(10);
    return;
  }

  uint32_t now = millis();
  bool freshSample = readOrientationSample(now);
  now = millis();

  if (freshSample)
  {
    switch (state)
    {
      case WAITING_FOR_NORMAL:
        if (normalHeldLongEnough(now))
        {
          enterReady(false);
        }
        break;

      case READY:
        // LED-only pre-cue: show the arrow as the prepared Token leaves
        // NORMAL, before the start threshold is necessarily reached.
        // This does not arm a session or change the sensor thresholds.
        if (zHistoryCount == Z_FILTER_SAMPLES && !isNormal())
        {
          beginArrowGuide(now);
        }
        if (isFlipped())
        {
          beginFlipAttempt(now);
        }
        break;

      case CONFIRMING_FLIP:
        if (isFlipped())
        {
          if (!orientationTiming)
          {
            orientationTiming = true;
            orientationStartTime = now;
            Serial.println("HOLD_RESTART: still armed, checking again.");
          }
          if (now - orientationStartTime >= CONFIRM_MS)
          {
            startSession();
          }
        }
        else
        {
          // Reset ONLY the hold timer. Remain in CONFIRMING_FLIP;
          // brief movement no longer sends the user to WAITING_FOR_NORMAL.
          if (orientationTiming)
          {
            Serial.println("HOLD_RESET: not started; still armed for retry.");
          }
          orientationTiming = false;
        }
        break;

      case COMPLETED:
        // Do not let an early return-to-normal erase the waiting/result UI.
        // Sensitivity is unchanged; only completion exit is gated.
        if (!completionCanReturnToReady(now))
        {
          orientationTiming = false;
          break;
        }
        if (normalHeldLongEnough(now))
        {
          enterReady(true);
        }
        break;

      case RUNNING:
        break;
    }
  }

  // Retain the same candidate timeout/retry logic. The LED guide persists
  // across this transition, rather than changing to X or the idle hourglass.
  now = millis();
  if (state == CONFIRMING_FLIP
      && now - flipAttemptStartTime >= FLIP_ATTEMPT_MS)
  {
    returnReadyForRetry(now);
  }

  // Keep blinking independently of sensor sample arrival. Sensor gaps
  // still reset the hold, exactly as in the uploaded working version.
  updateArrowGuide(now);
  if (state != RUNNING)
  {
    printOrientationDebug(now);
  }
  delay(10);
}
