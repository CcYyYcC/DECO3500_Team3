#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// TIME TOWER / R4 — DEMO CYCLE v3 / 5-second G-B-R closing breath
// Compatible with existing DemoCycle v3 Tower bridge and all three v3 Tokens.
// Only the round-end scene is replaced. The UART/radio protocol is unchanged.
// D0/RX <- Tower GPIO7; D1/TX -> Tower GPIO8; existing level converter retained.
// D6 -> 8x32 WS2812B; original colours/shapes/serpentine mapping retained.
// Real Tokens 1/2/3 ALL use LEFT. RIGHT is explicitly simulated, not user data.
// Real software reset: NVIC_SystemReset(), not external power switching.
constexpr uint8_t LED_PIN=6, WIDTH=8, HEIGHT=32, MAX_UNITS=10;
constexpr uint16_t NUM_LEDS=256, DROP_DELAY=70;
constexpr uint32_t UART_BAUD=9600;
constexpr uint8_t BRIGHTNESS=20; // field-tested setting: reduced full-panel random colours
constexpr uint32_t SIM_INTERVAL_MS=10000UL;  // first drop at 10s, then 20s,...,100s
constexpr uint32_t FULL_HOLD_MS=10000UL;      // from the 10th LEFT unit landing
// FADE_MS remains the internal name to preserve the v3 FADING/FADE_DONE protocol.
constexpr uint32_t FADE_MS=5000UL;           // TOTAL time for green -> blue -> red
constexpr uint8_t CLOSING_MAX_LEVEL=160;     // 0..255; additionally limited by BRIGHTNESS
// Three breaths have decreasing electrical peaks (100%, 75%, 50%).
// These percentages are not a perceptual brightness calibration across colours.
constexpr uint8_t CLOSING_PEAK_SCALE[3]={255,192,128};
constexpr uint32_t FRAME_MS=35UL;
constexpr bool ENABLE_RIGHT_SIMULATION=true;
static_assert(SIM_INTERVAL_MS>0 && FADE_MS>=3, "timing must be positive");
Adafruit_NeoPixel leds(NUM_LEDS, LED_PIN, NEO_GRB+NEO_KHZ800);
enum Activity : uint8_t { STUDY=1, EXERCISE=2, GAMING=3 };
const Activity SIM_SEQUENCE[MAX_UNITS]={STUDY,STUDY,GAMING,GAMING,GAMING,EXERCISE,STUDY,STUDY,STUDY,GAMING};
Activity person0Units[MAX_UNITS], person1Units[MAX_UNITS];
uint8_t person0Count=0,person1Count=0;
uint8_t lastClosingPhase=255; // USB log only, never changes the animation timing
struct DropRequest {uint32_t roundID, tokenID, bootID, activity, units, messageNumber;};
struct Falling {bool active=false; Activity activity=STUDY; int y=29,targetY=0; uint32_t lastStep=0;};
Falling falling[2];
struct Batch {bool active=false; DropRequest request={}; uint32_t landed=0,nextStart=0;};
Batch batch;
DropRequest completed[10]={}; uint8_t completedCount=0;
enum TowerMode : uint8_t { WAIT_ROUND, ACTIVE, HOLD_FULL, FADING, WAIT_REBOOT, REBOOT_PENDING };
TowerMode mode=WAIT_ROUND;
uint32_t roundID=0,nextRoundID=0,roundStartMs=0,nextSimMs=0,holdStartMs=0,fadeStartMs=0;
uint32_t lastStatusMs=0,lastFrameMs=0,rebootAtMs=0;
uint8_t simNext=0;
bool sceneDirty=true;
// NeoPixel show() may mask UART interrupts: grant a short quiet receive window.
bool uartQuiet=false;uint32_t uartQuietUntil=0;
char inputLine[128]={}; size_t inputLength=0; bool inputOverflow=false;

uint16_t xyToIndex(uint8_t x,uint8_t y);
void setXY(int x,int y,uint32_t color);
uint32_t getActivityColor(Activity activity);
void drawTimeUnit(int x,int y,Activity activity);
void drawTower();
const char* activityName(Activity activity);
bool parseCsv(const char *s,uint32_t *v,uint8_t n);
uint32_t closingColorAt(uint32_t elapsed);
void drawScene();
void startFall(uint8_t lane,Activity a,uint32_t now);
void stepFalls(uint32_t now);
void finishBatch(uint32_t now);
void beginFullHold(uint32_t now);
void replyResult(const DropRequest &r,bool done,const char *reason);
void handleLine(const char *line);
void handleDrop(const DropRequest &r);
void pollUART();
void updateRound(uint32_t now);
void sendStatus(uint32_t now,bool force=false);
void rebootR4();

uint16_t xyToIndex(
  uint8_t x,
  uint8_t y
)
{
  if (y % 2 == 0)
  {
    return y * 8 + x;
  }
  else
  {
    return y * 8 + (7 - x);
  }
}

void setXY(
  int x,
  int y,
  uint32_t color
)
{
  if (
    x < 0 ||
    x >= WIDTH ||
    y < 0 ||
    y >= HEIGHT
  )
  {
    return;
  }

  // Normal activity colours remain untouched. The closing animation has its
  // own full-panel renderer and never rescales these stored activity shapes.
  leds.setPixelColor(
    xyToIndex(x, y),
    color
  );
}

uint32_t getActivityColor(
  Activity activity
)
{
  switch (activity)
  {
    case STUDY:
      // Blue
      return leds.Color(
        40,
        120,
        255
      );

    case EXERCISE:
      // Green
      return leds.Color(
        90,
        255,
        20
      );

    case GAMING:
      // Red / pink
      return leds.Color(
        255,
        60,
        70
      );
  }

  return 0;
}

void drawTimeUnit(
  int startX,
  int startY,
  Activity activity
)
{
  uint32_t color =
    getActivityColor(activity);


  // -------------------------------------------------
  // STUDY
  //
  // XXX
  // X.X
  // XXX
  // -------------------------------------------------

  if (activity == STUDY)
  {
    for (int y = 0; y < 3; y++)
    {
      for (int x = 0; x < 3; x++)
      {
        // Skip center
        if (
          x == 1 &&
          y == 1
        )
        {
          continue;
        }

        setXY(
          startX + x,
          startY + y,
          color
        );
      }
    }
  }


  // -------------------------------------------------
  // EXERCISE
  //
  // XXX
  // .X.
  // XXX
  // -------------------------------------------------

  else if (activity == EXERCISE)
  {
    // Bottom row
    setXY(
      startX + 0,
      startY + 0,
      color
    );

    setXY(
      startX + 1,
      startY + 0,
      color
    );

    setXY(
      startX + 2,
      startY + 0,
      color
    );


    // Center
    setXY(
      startX + 1,
      startY + 1,
      color
    );


    // Top row
    setXY(
      startX + 0,
      startY + 2,
      color
    );

    setXY(
      startX + 1,
      startY + 2,
      color
    );

    setXY(
      startX + 2,
      startY + 2,
      color
    );
  }


  // -------------------------------------------------
  // GAMING
  //
  // .X.
  // XXX
  // .X.
  // -------------------------------------------------

  else if (activity == GAMING)
  {
    setXY(
      startX + 1,
      startY + 0,
      color
    );

    setXY(
      startX + 0,
      startY + 1,
      color
    );

    setXY(
      startX + 1,
      startY + 1,
      color
    );

    setXY(
      startX + 2,
      startY + 1,
      color
    );

    setXY(
      startX + 1,
      startY + 2,
      color
    );
  }
}

void drawTower()
{
  leds.clear();


  // -------------------------------------------------
  // Person 0
  // left side x = 0..2
  // -------------------------------------------------

  for (
    uint8_t i = 0;
    i < person0Count;
    i++
  )
  {
    drawTimeUnit(
      0,
      i * 3,
      person0Units[i]
    );
  }


  // -------------------------------------------------
  // Person 1
  // right side x = 5..7
  // -------------------------------------------------

  for (
    uint8_t i = 0;
    i < person1Count;
    i++
  )
  {
    drawTimeUnit(
      5,
      i * 3,
      person1Units[i]
    );
  }
}

const char* activityName(
  Activity activity
)
{
  switch (activity)
  {
    case STUDY:
      return "STUDY";

    case EXERCISE:
      return "EXERCISE";

    case GAMING:
      return "GAMING";
  }

  return "UNKNOWN";
}


// Strict parser: accepts only the requested number of unsigned CSV fields.
bool parseCsv(const char *s, uint32_t *v, uint8_t n) {
  for(uint8_t i=0;i<n;++i) {
    while(*s==' ' || *s=='\t') ++s;
    if(*s<'0'||*s>'9') return false;
    uint32_t x=0;
    while(*s>='0'&&*s<='9') {
      uint32_t d=uint32_t(*s-'0'); if(x>(UINT32_MAX-d)/10UL) return false;
      x=x*10UL+d; ++s;
    }
    while(*s==' ' || *s=='\t') ++s;
    v[i]=x;
    if(i+1<n) {if(*s!=',') return false; ++s;} else if(*s) return false;
  }
  return true;
}

// One pure colour for ALL 256 pixels, including the gap and top unused rows.
// Each ~1.67s phase brightens gently, then fades to black. The next colour
// starts from black. No white frame, no old shapes, no cumulative dimming.
uint32_t closingColorAt(uint32_t elapsed) {
  if(elapsed>=FADE_MS) return 0;

  // Fractional phase calculation keeps the TOTAL animation at FADE_MS even
  // when FADE_MS is not divisible by three. All arithmetic is rollover-safe
  // because elapsed is an unsigned difference, not an absolute deadline.
  const uint64_t scaled=uint64_t(elapsed)*3ULL;
  const uint8_t phase=uint8_t(scaled/FADE_MS); // 0=green, 1=blue, 2=red
  const float progress=float(scaled%FADE_MS)/float(FADE_MS);

  // 35% brighten, 65% darken: more time is spent slowly fading out.
  float ramp=progress<0.35f ? progress/0.35f : (1.0f-progress)/0.65f;
  if(ramp<0.0f) ramp=0.0f;
  if(ramp>1.0f) ramp=1.0f;
  const float smooth=ramp*ramp*(3.0f-2.0f*ramp); // soft turn at the peak
  const float peak=float(CLOSING_MAX_LEVEL)*CLOSING_PEAK_SCALE[phase]/255.0f;
  const uint8_t level=uint8_t(peak*smooth+0.5f);

  if(phase==0) return leds.Color(0,level,0);
  if(phase==1) return leds.Color(0,0,level);
  return leds.Color(level,0,0);
}

void drawScene() {
  if(mode==FADING) {
    // Write fresh RGB values each frame. Do not animate setBrightness().
    leds.fill(closingColorAt(uint32_t(millis()-fadeStartMs)));
  } else if(mode==WAIT_REBOOT || mode==REBOOT_PENDING) {
    // Never let the old tower shapes reappear while the reboot is coordinated.
    leds.clear();
  } else {
    drawTower();
    for(uint8_t lane=0;lane<2;++lane)
      if(falling[lane].active) drawTimeUnit(lane==0?0:5,falling[lane].y,falling[lane].activity);
  }
  leds.show(); sceneDirty=false;
}
void startFall(uint8_t lane,Activity a,uint32_t now) {
  Falling &f=falling[lane]; f.active=true; f.activity=a;
  f.y=29; f.targetY=(lane==0?person0Count:person1Count)*3;
  f.lastStep=now; sceneDirty=true;
  Serial.print(lane==0 ? "LEFT DROP: " : "RIGHT SIMULATED DROP: "); Serial.println(activityName(a));
}
void replyResult(const DropRequest &r,bool done,const char *reason) {
  char line[128];
  int n=snprintf(line,sizeof(line),"%s,%lu,%lu,%lu,%lu,%lu,%lu%s%s\r\n",
    done?"DROP_DONE":"DROP_FAIL",(unsigned long)r.roundID,(unsigned long)r.tokenID,
    (unsigned long)r.bootID,(unsigned long)r.activity,(unsigned long)r.units,
    (unsigned long)r.messageNumber,done?"":",",done?"":reason);
  if(n<=0||n>=int(sizeof(line))){Serial.println("ERROR: reply too long");return;}
  Serial1.write((const uint8_t*)line,size_t(n));
  Serial.print("UART result -> Tower: ");Serial.print(line);
}
void finishBatch(uint32_t now) {
  const DropRequest r=batch.request;
  batch.active=false;
  completed[completedCount++]=r;
  // Final frame reaches the LEDs before the software processing ACK.
  drawScene(); replyResult(r,true,nullptr);
  if(person0Count==MAX_UNITS) beginFullHold(now);
}
void beginFullHold(uint32_t now) {
  if(mode!=ACTIVE) return;
  mode=HOLD_FULL; holdStartMs=now;
  Serial.println("LEFT FULL: hold 10 seconds, then full-panel GREEN -> BLUE -> RED breath (5s total).");
  sendStatus(now,true);
}
void stepFalls(uint32_t now) {
  if(mode!=ACTIVE && mode!=HOLD_FULL) return;
  for(uint8_t lane=0;lane<2;++lane) {
    Falling &f=falling[lane];
    if(!f.active || now-f.lastStep<DROP_DELAY) continue;
    f.lastStep=now;
    if(f.y>f.targetY){--f.y;sceneDirty=true;continue;}
    f.active=false;
    if(lane==0) {
      person0Units[person0Count++]=f.activity;
      ++batch.landed; sceneDirty=true;
      if(batch.landed>=batch.request.units) finishBatch(now);
      else batch.nextStart=now+250UL;
    } else {
      person1Units[person1Count++]=f.activity; sceneDirty=true;
      Serial.print("SIMULATED right units = ");Serial.println(person1Count);
    }
  }
  if(mode==ACTIVE && batch.active && !falling[0].active
      && int32_t(now-batch.nextStart)>=0) startFall(0,Activity(batch.request.activity),now);
  // Independent scheduling: a real drop does not delay the simulated lane.
  if(ENABLE_RIGHT_SIMULATION && simNext<MAX_UNITS && !falling[1].active
      && int32_t(now-nextSimMs)>=0 && (mode==ACTIVE||mode==HOLD_FULL)) {
    startFall(1,SIM_SEQUENCE[simNext++],now);
    nextSimMs=roundStartMs+uint32_t(simNext+1)*SIM_INTERVAL_MS;
  }
}
void handleDrop(const DropRequest &r) {
  if(r.roundID!=roundID || roundID==0){replyResult(r,false,"STALE_ROUND");return;}
  if(r.tokenID<1||r.tokenID>3){replyResult(r,false,"UNKNOWN_TOKEN");return;}
  if(r.activity<STUDY||r.activity>GAMING){replyResult(r,false,"BAD_ACTIVITY");return;}
  if(r.units==0||r.units>MAX_UNITS||r.bootID==0||r.messageNumber==0){replyResult(r,false,"BAD_REQUEST");return;}
  for(uint8_t i=0;i<completedCount;++i) {
    const DropRequest &old=completed[i];
    if(old.roundID==r.roundID && old.tokenID==r.tokenID && old.bootID==r.bootID && old.messageNumber==r.messageNumber) {
      if(old.activity==r.activity && old.units==r.units) replyResult(r,true,nullptr);
      else replyResult(r,false,"MESSAGE_CONFLICT");
      return;
    }
  }
  if(mode!=ACTIVE){replyResult(r,false,"ROUND_CLOSING");return;}
  if(batch.active) {
    const DropRequest &old=batch.request;
    if(old.tokenID==r.tokenID&&old.bootID==r.bootID&&old.messageNumber==r.messageNumber
       &&old.activity==r.activity&&old.units==r.units) return;
    replyResult(r,false,"BUSY");return;
  }
  if(r.units>uint32_t(MAX_UNITS-person0Count)){replyResult(r,false,"FULL");return;}
  batch.active=true;batch.request=r;batch.landed=0;
  startFall(0,Activity(r.activity),millis());
}
void rebootR4() {
  leds.clear();leds.show();
  Serial.println("R4 SOFTWARE REBOOT NOW; volatile scene/counters will reset.");
  Serial1.flush();delay(30);
  NVIC_SystemReset();
}
void handleLine(const char *line) {
  uint32_t v[6];
  if(strncmp(line,"ROUND,",6)==0 && parseCsv(line+6,v,1) && v[0]!=0) {
    if(roundID==0) {
      roundID=v[0]; mode=ACTIVE;roundStartMs=millis();nextSimMs=roundStartMs+SIM_INTERVAL_MS;
      Serial.print("ROUND ACTIVE: ");Serial.println(roundID);
      sendStatus(millis(),true);
    } else if(v[0]!=roundID && mode!=REBOOT_PENDING) {
      // Bridge rebooted unexpectedly: discard the old demonstration, not data into a wrong round.
      Serial.println("Tower epoch changed; R4 rebooting to rejoin.");rebootR4();
    } else if(mode==ACTIVE) sendStatus(millis(),true);
    return;
  }
  if(strncmp(line,"DROP,",5)==0 && parseCsv(line+5,v,6)) {
    const DropRequest r={v[0],v[1],v[2],v[3],v[4],v[5]};handleDrop(r);return;
  }
  if(strncmp(line,"REBOOT,",7)==0 && parseCsv(line+7,v,3)) {
    // old epoch, new epoch, remaining countdown. Only honour an expected round close.
    if(v[0]!=roundID||v[1]==0||v[1]==roundID||v[2]>10000UL
        ||(mode!=WAIT_REBOOT&&mode!=REBOOT_PENDING)) return;
    uint32_t candidate=millis()+v[2];
    if(mode!=REBOOT_PENDING || int32_t(candidate-rebootAtMs)<0) rebootAtMs=candidate;
    nextRoundID=v[1];mode=REBOOT_PENDING;
    Serial1.print("REBOOT_READY,");Serial1.println(nextRoundID);Serial1.flush();
    return;
  }
  Serial.print("Ignored UART command (check v3 sketches): ");Serial.println(line);
}
void pollUART() {
  while(Serial1.available()>0) {
    int c=Serial1.read();if(c<0)break;if(c=='\r')continue;
    if(c=='~') {
      inputLength=0;inputOverflow=false;uartQuiet=true;uartQuietUntil=millis()+250UL;
      Serial1.print("UART_READY,");Serial1.println(roundID);Serial1.flush();
      continue;
    }
    if(c=='\n') {
      if(!inputOverflow&&inputLength){inputLine[inputLength]='\0';uartQuiet=false;handleLine(inputLine);}
      else if(inputOverflow)Serial.println("UART line invalid/too long; ignored.");
      inputLength=0;inputOverflow=false;continue;
    }
    if(inputOverflow)continue;
    if(c==0||inputLength+1>=sizeof(inputLine))inputOverflow=true;
    else inputLine[inputLength++]=char(c);
  }
}
void sendStatus(uint32_t now,bool force) {
  if(!force && now-lastStatusMs<500UL)return;lastStatusMs=now;
  const char *s= mode==WAIT_ROUND?"R4_HELLO":mode==ACTIVE?"R4_READY":
      mode==HOLD_FULL?"ROUND_FULL":mode==FADING?"FADING":mode==WAIT_REBOOT?"FADE_DONE":"REBOOT_READY";
  Serial1.print(s);Serial1.print(',');Serial1.println(mode==REBOOT_PENDING?nextRoundID:roundID);
}
void updateRound(uint32_t now) {
  if(mode==HOLD_FULL && now-holdStartMs>=FULL_HOLD_MS) {
    mode=FADING;fadeStartMs=now;lastClosingPhase=255;sceneDirty=true;
    Serial.print("FULL PANEL BREATH START: GREEN -> BLUE -> RED; total ms=");
    Serial.println(FADE_MS);
    sendStatus(now,true); // keep the existing v3 FADING message
  }
  if(mode==FADING) {
    const uint32_t elapsed=now-fadeStartMs;
    if(elapsed>=FADE_MS) {
      mode=WAIT_REBOOT;
      // Blank physically before notifying the bridge. This is a one-off
      // refresh, not a blocking five-second loop. UART is polled normally.
      drawScene();
      Serial.println("FULL PANEL BREATH DONE: BLACK. Waiting for coordinated reboot.");
      sendStatus(now,true); // FADE_DONE still starts the v3 reboot handshake
    } else {
      const uint8_t phase=uint8_t(uint64_t(elapsed)*3ULL/FADE_MS);
      if(phase!=lastClosingPhase) {
        lastClosingPhase=phase;
        Serial.print("CLOSING COLOUR: ");
        Serial.println(phase==0?"GREEN":phase==1?"BLUE":"RED");
      }
      sceneDirty=true; // rendered by loop() at FRAME_MS, not every iteration
    }
  }
  if(mode==REBOOT_PENDING && int32_t(now-rebootAtMs)>=0)rebootR4();
}
void setup() {
  Serial.begin(115200);Serial1.begin(UART_BAUD);
  leds.begin();leds.setBrightness(BRIGHTNESS);leds.clear();leds.show();
  delay(1000);
  Serial.println("TIME TOWER R4: DEMO CYCLE v3 / FULL PANEL BREATH 5s");
  Serial.println("Closing only: GREEN -> BLUE -> RED -> BLACK; existing v3 reboot preserved.");
  Serial.println("LEFT=real Tokens 1/2/3; RIGHT=SIMULATED S,S,G,G,G,E,S,S,S,G every10s.");
  Serial.println("Waiting for v3 Tower bridge ROUND. Do not mix old firmware.");
  sendStatus(millis(),true);
}
void loop() {
  pollUART();const uint32_t now=millis();
  updateRound(now);stepFalls(now);
  if(uartQuiet && int32_t(now-uartQuietUntil)>=0)uartQuiet=false;
  if(sceneDirty && !uartQuiet && now-lastFrameMs>=FRAME_MS){lastFrameMs=now;drawScene();}
  sendStatus(now);delay(1);
}
