///////////////////////////////////////////////////////
// warpcore firmare
// written by c-lab @ c-base
// https://github.com/c-base/warpcore
// GNU GENERAL PUBLIC LICENSE
///////////////////////////////////////////////////////

#include <Wtv020sd16p.h>

//////////////////  PINS  /////////////////////////////
#define AUDIO_BACKGROUND_RESET_PIN  2
#define AUDIO_BACKGROUND_CLOCK_PIN  3
#define AUDIO_BACKGROUND_DATA_PIN   4
#define AUDIO_BACKGROUND_BUSY_PIN   5

#define AUDIO_TX_RESET_PIN          2
#define AUDIO_TX_CLOCK_PIN          3
#define AUDIO_TX_DATA_PIN           4
#define AUDIO_TX_BUSY_PIN           5

#define LED_DATA_PIN                8
#define LED_CLOCK_PIN               9
#define LED_ENABLE_PIN             10

#define LINE_DELAY_PIN             A4
#define LINE_SPEED_PIN             A5

/////////////////// LEDs //////////////////////////
#define NUM_LED_LINES              18

////////////////// SCHEDULER //////////////////////
#define NUM_SCHEDULERS              3
#define SCHEDULE_BACKGROUND         0
#define SCHEDULE_TX                 1
#define SCHEDULE_LIGHT              2

#define INIT_INTERVAL_BACKGROUND  500
#define INIT_INTERVAL_TX          500
#define INIT_INTERVAL_LIGHT        10
//////////////////////////////////////////////////

///////////////////// DEFS ///////////////////////
typedef void (*funcptr)();
typedef struct {
  uint64_t lastCall;
  uint32_t interval;
  funcptr  function;
} scheduler_t;

/////////////////// global vars //////////////////
scheduler_t scheduler[NUM_SCHEDULERS];
uint8_t     ledCounter = 0;
Wtv020sd16p backgroundModule(AUDIO_BACKGROUND_RESET_PIN, AUDIO_BACKGROUND_CLOCK_PIN, AUDIO_BACKGROUND_DATA_PIN, AUDIO_BACKGROUND_BUSY_PIN);
Wtv020sd16p txModule        (AUDIO_TX_RESET_PIN,         AUDIO_TX_CLOCK_PIN,         AUDIO_TX_DATA_PIN,         AUDIO_TX_BUSY_PIN        );

//////////////////// SETUP ///////////////////////
void setup() {
  
  // Background
  scheduler[SCHEDULE_BACKGROUND].lastCall = 0;
  scheduler[SCHEDULE_BACKGROUND].interval = INIT_INTERVAL_BACKGROUND;
  scheduler[SCHEDULE_BACKGROUND].function = schedulerBackground;
  // TX
  scheduler[SCHEDULE_TX].lastCall         = 0;
  scheduler[SCHEDULE_TX].interval         = INIT_INTERVAL_TX;
  scheduler[SCHEDULE_TX].function         = schedulerTX;
  // LIGHT
  scheduler[SCHEDULE_LIGHT].lastCall      = 0;
  scheduler[SCHEDULE_LIGHT].interval      = INIT_INTERVAL_LIGHT;
  scheduler[SCHEDULE_LIGHT].function      = schedulerLight;  
  
  pinMode(LED_DATA_PIN,   OUTPUT);
  pinMode(LED_CLOCK_PIN,  OUTPUT);
  pinMode(LED_ENABLE_PIN, OUTPUT);

  backgroundModule.reset();
  txModule.reset();

  ledAll(false);
  digitalWrite(LED_ENABLE_PIN, true);
}

/////////////////// LOOP //////////////////////
void loop() {
  for(uint8_t i = 0; i < NUM_SCHEDULERS; i++){
    if((scheduler[i].lastCall + scheduler[i].interval) < millis()){
      scheduler[i].function();
      scheduler[i].lastCall = millis();
    }
  }
}

//////////////// SCHEDULER FUNCTIONS /////////////
void schedulerBackground(){
  backgroundModule.playVoice(0);
  backgroundModule.asyncPlayVoice(1);
  backgroundModule.pauseVoice();
  backgroundModule.stopVoice();
}

void schedulerTX(){
  txModule.playVoice(0);
  txModule.asyncPlayVoice(1);
  txModule.pauseVoice();
  txModule.stopVoice();
  scheduler[SCHEDULE_TX].interval = getDelay();  
}

void schedulerLight(){
  ledNext();
  scheduler[SCHEDULE_LIGHT].interval = getSpeed();  
}

//////////////// LIGHT HELPER /////////////////////
uint32_t getDelay(){
  return exp(analogRead(LINE_DELAY_PIN) * 0.01);
}

uint32_t getSpeed(){
  return exp(analogRead(LINE_SPEED_PIN) * 0.01);
}

void ledTick(bool onoff){
  digitalWrite(LED_DATA_PIN,  onoff);
  digitalWrite(LED_CLOCK_PIN, true);
  delay(1);  //TODO check if neccessary
  digitalWrite(LED_CLOCK_PIN, false);
}

void ledAll(bool status){
 for (uint8_t i = 0; i < NUM_LED_LINES; i++){
   ledTick(status);
 }  
}

void ledNext(){
  if(ledCounter++ > NUM_LED_LINES){
    ledTick(false);
    ledCounter = 0;
  }else{
    ledTick(true);
  } 
}
