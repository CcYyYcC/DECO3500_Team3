#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <esp_err.h>
#include <Preferences.h>
#include <cstring>
#include <cstdio>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

// TOWER ESP32 BRIDGE v3. GPIO7 TX / GPIO8 RX; R4 D0/D1, 9600.
// Supports three Tokens + R4 autonomous simulated right column.
// The final result still means R4 executed a scene update, NOT nonvolatile storage.
// A separate round-state packet coordinates real software reboots of all online boards.
constexpr uint8_t ESPNOW_CHANNEL=6, UART_TX_PIN=7, UART_RX_PIN=8;
constexpr uint32_t UART_BAUD=9600, DROP_TIMEOUT_MS=30000UL;
constexpr uint32_t STATE_INTERVAL_MS=500UL, REBOOT_WINDOW_MS=2000UL;
constexpr bool SEND_RECEIVER_ACK=true, SEND_FINAL_RESULT=true;
HardwareSerial R4Serial(1);
uint8_t broadcastAddress[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

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

QueueHandle_t radioQueue=nullptr;
struct Peer {bool known=false;uint8_t mac[6]={};uint32_t seen=0;};
Peer tokens[4];
constexpr uint8_t JOB_CAP=8;
RadioEvent jobs[JOB_CAP],activeJob;
uint8_t jobHead=0,jobTail=0,jobCount=0;
bool waitingForDrop=false,dropTimedOut=false,r4Ready=false;
bool waitingForUartReady=false;uint32_t uartReadyStarted=0,lastKnockMs=0;
uint32_t roundID=0,nextRoundID=0,dropStartMs=0,lastStateMs=0,lastRoundSendMs=0,rebootAtMs=0;
DemoPhase demoPhase=DP_STARTING;
bool r4RebootReady=false;
uint8_t tokenRebootMask=0,onlineMaskAtReset=0;
char returnLine[160]={};size_t returnLength=0;bool returnOverflow=false;

void fatalError(const char *s);
bool ensurePeer(const uint8_t *mac);
void onRadio(const esp_now_recv_info_t *info,const uint8_t *data,int len);
void sendState(const uint8_t *mac=nullptr);
void sendAck(const RadioEvent &event,uint8_t status);
void handleRadio(const RadioEvent &event);
void handleR4(const char *line);
void pollR4();
void startNext();
void dispatchActive();
void abandonQueued(uint8_t status);
void enterClosing(DemoPhase phase);
void beginReboot();
void updateControl(uint32_t now);
bool parseCsv(const char *s,uint32_t *v,uint8_t n);
void sendRound();

void fatalError(const char *s){while(true){Serial.println(s);delay(2000);}}
bool ensurePeer(const uint8_t *mac) {
  if(esp_now_is_peer_exist(mac))return true;
  esp_now_peer_info_t peer={};memcpy(peer.peer_addr,mac,6);
  peer.channel=ESPNOW_CHANNEL;peer.ifidx=WIFI_IF_STA;peer.encrypt=false;
  esp_err_t e=esp_now_add_peer(&peer);
  if(e!=ESP_OK&&e!=ESP_ERR_ESPNOW_EXIST) {Serial.print("Peer error: ");Serial.println(esp_err_to_name(e));return false;}
  return true;
}
void onRadio(const esp_now_recv_info_t *info,const uint8_t *data,int len) {
  if(!radioQueue||!info||!data||len!=int(sizeof(DemoPacket)))return;
  RadioEvent e={};memcpy(&e.p,data,sizeof(e.p));memcpy(e.mac,info->src_addr,6);
  if(!envelopeOK(e.p))return;
  if(e.p.kind!=PK_DATA && e.p.kind!=PK_HELLO && e.p.kind!=PK_RESET_READY)return;
  xQueueSend(radioQueue,&e,0); // short callback: no Serial, UART, restart or LED work
}
void sendState(const uint8_t *mac) {
  const uint8_t *target=mac?mac:broadcastAddress;
  if(!ensurePeer(target))return;
  DemoPacket p=makePacket(PK_STATE);p.roundID=demoPhase==DP_RESETTING?nextRoundID:roundID;
  p.status=demoPhase;
  p.argument=(demoPhase==DP_RESETTING && !deadlineReached(millis(),rebootAtMs))?rebootAtMs-millis():0;
  esp_now_send(target,reinterpret_cast<const uint8_t*>(&p),sizeof(p));
}
void sendAck(const RadioEvent &event,uint8_t status) {
  if((status==ACK_TOWER_RECEIVED&&!SEND_RECEIVER_ACK)||(status!=ACK_TOWER_RECEIVED&&!SEND_FINAL_RESULT))return;
  if(!ensurePeer(event.mac))return;
  DemoPacket p=event.p;p.kind=PK_ACK;p.status=status;p.argument=0;
  esp_err_t r=esp_now_send(event.mac,reinterpret_cast<const uint8_t*>(&p),sizeof(p));
  Serial.printf("%s QUEUED=%s: round=%lu token=%u boot=%lu msg=%lu status=%u\n",
    status==1?"RECEIVER ACK":"FINAL ACK",r==ESP_OK?"YES":"NO",(unsigned long)p.roundID,
    unsigned(p.tokenID),(unsigned long)p.bootID,(unsigned long)p.messageNumber,unsigned(status));
}
void abandonQueued(uint8_t status) {
  while(jobCount){sendAck(jobs[jobHead],status);jobHead=(jobHead+1)%JOB_CAP;--jobCount;}
  jobTail=jobHead;
}
void enterClosing(DemoPhase phase) {
  if(demoPhase==DP_RESETTING)return;
  if(phase<demoPhase)return;
  if(demoPhase!=phase) {
    demoPhase=phase;abandonQueued(ACK_R4_REJECTED);
    if(waitingForUartReady){sendAck(activeJob,ACK_R4_REJECTED);waitingForUartReady=false;}
    Serial.println(phase==DP_HOLD?"ROUND CLOSING: LEFT reached10. Block new sessions; 10s hold.":"TOWER UNIFORM FADE.");
    sendState();lastStateMs=millis();
  }
}
void handleRadio(const RadioEvent &e) {
  const DemoPacket &p=e.p;
  if(p.tokenID<1||p.tokenID>3||p.activity<1||p.activity>3||p.bootID==0)return;
  Peer &peer=tokens[p.tokenID];
  if(peer.known && memcmp(peer.mac,e.mac,6)!=0 && millis()-peer.seen<15000UL) {
    Serial.println("TOKEN_ID_CONFLICT: two live boards share an ID. Ignored.");return;
  }
  if(!peer.known || memcmp(peer.mac,e.mac,6)!=0) {
    peer.known=true;memcpy(peer.mac,e.mac,6);
    Serial.printf("TOKEN REGISTERED: %u, activity=%u\n",unsigned(p.tokenID),unsigned(p.activity));
  }
  peer.seen=millis();
  if(p.kind==PK_HELLO){sendState(e.mac);return;}
  if(p.kind==PK_RESET_READY) {
    if(demoPhase==DP_RESETTING&&p.roundID==nextRoundID){tokenRebootMask|=1U<<(p.tokenID-1);
      Serial.printf("Token %u accepted reboot for round %lu.\n",unsigned(p.tokenID),(unsigned long)nextRoundID);}
    return;
  }
  if(p.roundID!=roundID){Serial.println("STALE_ROUND DATA ignored; advertise current round.");sendState(e.mac);return;}
  if(p.messageNumber==0||p.newUnits==0||p.newUnits>p.totalUnits)return;
  sendAck(e,ACK_TOWER_RECEIVED);
  if(demoPhase!=DP_ACTIVE||!r4Ready){sendAck(e,ACK_R4_REJECTED);return;}
  if((waitingForDrop||waitingForUartReady) && sameKey(activeJob.p,p)) {
    if(!sameMessage(activeJob.p,p)||dropTimedOut)sendAck(e,ACK_RESULT_UNKNOWN);return;
  }
  if(waitingForDrop&&dropTimedOut){sendAck(e,ACK_RESULT_UNKNOWN);return;}
  for(uint8_t i=0;i<jobCount;++i) if(sameKey(jobs[(jobHead+i)%JOB_CAP].p,p)) {
    if(!sameMessage(jobs[(jobHead+i)%JOB_CAP].p,p))sendAck(e,ACK_RESULT_UNKNOWN);return;
  }
  if(jobCount>=JOB_CAP){sendAck(e,ACK_R4_REJECTED);return;}
  jobs[jobTail]=e;jobTail=(jobTail+1)%JOB_CAP;++jobCount;
  Serial.printf("REAL TOKEN queued: id=%u activity=%u units=%u message=%lu round=%lu\n",
    unsigned(p.tokenID),unsigned(p.activity),unsigned(p.newUnits),(unsigned long)p.messageNumber,(unsigned long)p.roundID);
}
void startNext() {
  if(demoPhase!=DP_ACTIVE||!r4Ready||waitingForDrop||waitingForUartReady||!jobCount)return;
  activeJob=jobs[jobHead];jobHead=(jobHead+1)%JOB_CAP;--jobCount;
  waitingForUartReady=true;uartReadyStarted=millis();lastKnockMs=millis();
  // One safe-to-repeat wake byte; wait until the R4 stops NeoPixel refresh before sending CSV.
  R4Serial.print('~');
}
void dispatchActive() {
  waitingForUartReady=false;
  const DemoPacket &p=activeJob.p;
  char line[112];int n=snprintf(line,sizeof(line),"DROP,%lu,%u,%lu,%u,%u,%lu\r\n",
    (unsigned long)p.roundID,unsigned(p.tokenID),(unsigned long)p.bootID,unsigned(p.activity),
    unsigned(p.newUnits),(unsigned long)p.messageNumber);
  if(n<=0||n>=int(sizeof(line))){sendAck(activeJob,ACK_RESULT_UNKNOWN);return;}
  waitingForDrop=true;dropTimedOut=false;dropStartMs=millis();
  size_t written=R4Serial.write((const uint8_t*)line,size_t(n));
  Serial.print("UART -> R4: ");Serial.print(line);
  if(written!=size_t(n))Serial.println("WARNING: UART write incomplete; waiting, no auto retry.");
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

void sendRound(){R4Serial.print("ROUND,");R4Serial.println(roundID);lastRoundSendMs=millis();}
void beginReboot() {
  if(demoPhase==DP_RESETTING)return;
  // Preference is incremented ON BOOT. This is the epoch the next boot will use.
  if(roundID==UINT32_MAX)fatalError("Round counter exhausted; manual maintenance needed.");
  nextRoundID=roundID+1;demoPhase=DP_RESETTING;
  abandonQueued(ACK_R4_REJECTED);
  if(waitingForDrop||waitingForUartReady){sendAck(activeJob,ACK_RESULT_UNKNOWN);waitingForDrop=false;waitingForUartReady=false;}
  rebootAtMs=millis()+REBOOT_WINDOW_MS;r4RebootReady=false;tokenRebootMask=0;onlineMaskAtReset=0;
  for(uint8_t id=1;id<=3;++id)if(tokens[id].known&&millis()-tokens[id].seen<15000UL)onlineMaskAtReset|=1U<<(id-1);
  Serial.printf("REBOOT ROUND %lu -> %lu, online token mask=%u (1=Exercise,2=Study,4=Gaming).\n",
    (unsigned long)roundID,(unsigned long)nextRoundID,unsigned(onlineMaskAtReset));
  lastStateMs=millis()-STATE_INTERVAL_MS;
}
void handleR4(const char *line) {
  uint32_t v[6];
  if(strncmp(line,"UART_READY,",11)==0 && parseCsv(line+11,v,1)) {
    if(v[0]==roundID && waitingForUartReady && demoPhase==DP_ACTIVE)dispatchActive();
    return;
  }
  if(strncmp(line,"R4_HELLO,",9)==0 && parseCsv(line+9,v,1)) {
    if(r4Ready && demoPhase==DP_ACTIVE) {
      // A manual R4 restart lost its RAM. Start a new system epoch instead of reusing acknowledgements.
      Serial.println("R4 restarted manually; reboot bridge into a fresh round.");delay(50);esp_restart();
    }
    if(demoPhase!=DP_RESETTING)sendRound();return;
  }
  if(strncmp(line,"R4_READY,",9)==0 && parseCsv(line+9,v,1)) {
    if(v[0]==roundID && demoPhase==DP_STARTING){r4Ready=true;demoPhase=DP_ACTIVE;Serial.println("ROUND ACTIVE: R4 synchronized.");sendState();}
    return;
  }
  if(strncmp(line,"ROUND_FULL,",11)==0 && parseCsv(line+11,v,1)) {
    if(v[0]==roundID)enterClosing(DP_HOLD);return;
  }
  if(strncmp(line,"FADING,",7)==0 && parseCsv(line+7,v,1)) {
    if(v[0]==roundID)enterClosing(DP_FADE);return;
  }
  if(strncmp(line,"FADE_DONE,",10)==0 && parseCsv(line+10,v,1)) {
    if(v[0]==roundID)beginReboot();return;
  }
  if(strncmp(line,"REBOOT_READY,",13)==0 && parseCsv(line+13,v,1)) {
    if(demoPhase==DP_RESETTING&&v[0]==nextRoundID)r4RebootReady=true;return;
  }
  const bool done=strncmp(line,"DROP_DONE,",10)==0;
  const bool fail=strncmp(line,"DROP_FAIL,",10)==0;
  if(!done&&!fail){Serial.print("Ignored R4 line (not v3 result): ");Serial.println(line);return;}
  char numeric[112];const char *payload=line+10;const char *reason="";
  if(fail) {
    const char *comma=strrchr(payload,',');
    if(!comma||comma[1]=='\0'||size_t(comma-payload)>=sizeof(numeric))return;
    memcpy(numeric,payload,size_t(comma-payload));numeric[comma-payload]='\0';reason=comma+1;payload=numeric;
  }
  if(!parseCsv(payload,v,6)||!waitingForDrop)return;
  const DemoPacket &p=activeJob.p;
  if(v[0]!=p.roundID||v[1]!=p.tokenID||v[2]!=p.bootID||v[3]!=p.activity||v[4]!=p.newUnits||v[5]!=p.messageNumber) {
    Serial.println("UNMATCHED R4 result ignored.");return;
  }
  Serial.println(done?"=== R4 DROP CONFIRMED ===":"=== R4 DROP REJECTED ===");
  Serial.println(line);
  uint8_t status=done?ACK_R4_DROP_DONE:strcmp(reason,"MESSAGE_CONFLICT")==0?ACK_RESULT_UNKNOWN:ACK_R4_REJECTED;
  sendAck(activeJob,status);waitingForDrop=false;dropTimedOut=false;
}
void pollR4() {
  while(R4Serial.available()>0) {
    int c=R4Serial.read();if(c<0)break;if(c=='\r')continue;
    if(c=='\n') {
      if(!returnOverflow&&returnLength){returnLine[returnLength]='\0';handleR4(returnLine);}
      else if(returnOverflow)Serial.println("Bad R4 line discarded.");
      returnLength=0;returnOverflow=false;continue;
    }
    if(returnOverflow)continue;
    if(c==0||returnLength+1>=sizeof(returnLine))returnOverflow=true;
    else returnLine[returnLength++]=char(c);
  }
}
void updateControl(uint32_t now) {
  if(waitingForUartReady) {
    if(now-uartReadyStarted>=2000UL) {
      waitingForUartReady=false;sendAck(activeJob,ACK_RESULT_UNKNOWN);
      Serial.println("UART receive-window handshake timed out; DROP not sent.");
    } else if(now-lastKnockMs>=100UL) {lastKnockMs=now;R4Serial.print('~');}
  }
  if(waitingForDrop && !dropTimedOut && now-dropStartMs>=DROP_TIMEOUT_MS) {
    dropTimedOut=true;sendAck(activeJob,ACK_RESULT_UNKNOWN);abandonQueued(ACK_RESULT_UNKNOWN);
    Serial.println("R4 timeout: result UNKNOWN, dispatch paused; no auto resend.");
  }
  if(demoPhase==DP_STARTING && now-lastRoundSendMs>=500UL)sendRound();
  if(now-lastStateMs>=STATE_INTERVAL_MS) {
    lastStateMs=now;sendState();
    if(demoPhase==DP_RESETTING) {
      uint32_t remain=deadlineReached(now,rebootAtMs)?0:rebootAtMs-now;
      char line[64];snprintf(line,sizeof(line),"REBOOT,%lu,%lu,%lu\r\n",
        (unsigned long)roundID,(unsigned long)nextRoundID,(unsigned long)remain);R4Serial.print(line);
    }
  }
  if(demoPhase==DP_RESETTING && deadlineReached(now,rebootAtMs+100UL) && r4RebootReady) {
    uint8_t missing=onlineMaskAtReset&~tokenRebootMask;
    Serial.printf("BRIDGE REBOOT NOW; reset-ready mask=%u, unconfirmed mask=%u.\n",unsigned(tokenRebootMask),unsigned(missing));
    if(missing)Serial.println("Some Tokens did not ACK reboot; new-round beacon will resync them when reachable.");
    delay(50);esp_restart();
  }
}
void setup() {
  Serial.begin(115200);R4Serial.begin(UART_BAUD,SERIAL_8N1,UART_RX_PIN,UART_TX_PIN);delay(1000);
  Preferences prefs;
  if(!prefs.begin("tt_demo",false))fatalError("Cannot open round preferences.");
  uint32_t old=prefs.getUInt("round",0);
  if(old==UINT32_MAX)fatalError("Round counter exhausted.");
  roundID=old+1;
  if(prefs.putUInt("round",roundID)!=sizeof(uint32_t))fatalError("Cannot persist new round ID.");prefs.end();
  Serial.printf("TOWER BRIDGE DEMO CYCLE v3, ROUND=%lu\n",(unsigned long)roundID);
  Serial.println("All online Tokens + R4 + bridge software-reboot after LEFT10 + hold10s + fade2s.");
  radioQueue=xQueueCreate(16,sizeof(RadioEvent));if(!radioQueue)fatalError("Radio queue allocation failed.");
  if(!WiFi.mode(WIFI_STA))fatalError("WiFi mode failed.");delay(100);
  if(esp_wifi_set_channel(ESPNOW_CHANNEL,WIFI_SECOND_CHAN_NONE)!=ESP_OK)fatalError("WiFi channel failed.");
  if(esp_now_init()!=ESP_OK)fatalError("ESP-NOW init failed.");
  if(!ensurePeer(broadcastAddress))fatalError("Broadcast peer failed.");
  if(esp_now_register_recv_cb(onRadio)!=ESP_OK)fatalError("ESP-NOW callback failed.");
  Serial.print("Tower MAC: ");Serial.println(WiFi.macAddress());
  sendRound();sendState();lastStateMs=millis();
}
void loop() {
  pollR4();RadioEvent e;
  // Limited work per loop: UART and countdown remain responsive.
  for(uint8_t i=0;i<4 && xQueueReceive(radioQueue,&e,0)==pdTRUE;++i)handleRadio(e);
  updateControl(millis());startNext();delay(2);
}
