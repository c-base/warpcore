#include <Wtv020sd16p.h>

#define RESET_PIN       2
#define CLOCK_PIN       3
#define DATA_PIN        4
#define BUSY_PIN        5

#define LED_DATA_PIN    8
#define LED_CLOCK_PIN   9
#define LED_ENABLE_PIN  10

#define LINE_DELAY A4
#define LINE_SPEED A5

#define LED_LINES 16

#define NUM_SCHEDULERS 3

uint64_t scheduler[NUM_SCHEDULERS];
uint8_t ledCounter = 0;

Wtv020sd16p wtv020sd16p(RESET_PIN, CLOCK_PIN, DATA_PIN, BUSY_PIN);

void setup() {
  wtv020sd16p.reset();

  pinMode(LED_DATA_PIN, OUTPUT);
  pinMode(LED_CLOCK_PIN, OUTPUT);
  pinMode(LED_ENABLE_PIN, OUTPUT);

  ledOff();

  for(uint8_t i = 0; i < NUM_SCHEDULERS; i++){
    scheduler[i] = millis();
  }
    
}

void loop() {
  // Scheduler
  playBackground();
  for(uint8_t i = 0; i < NUM_SCHEDULERS; i++){
  }
  
  ledNext();
  delay(getSpeed());  
  
}

void playBackground(){
  wtv020sd16p.playVoice(0);
  wtv020sd16p.asyncPlayVoice(1);
  delay(5000);
  wtv020sd16p.pauseVoice();
  delay(5000);
  wtv020sd16p.pauseVoice();
  delay(5000);  
  wtv020sd16p.stopVoice();
}

uint16_t getDelay(){
  return exp(analogRead(LINE_DELAY) * 0.01);
}

uint16_t getSpeed(){
  return exp(analogRead(LINE_SPEED) * 0.01);
}

void ledTick(bool onoff){
  digitalWrite(LED_DATA_PIN, onoff);
  digitalWrite(LED_CLOCK_PIN, true);
  delay(1);
  digitalWrite(LED_CLOCK_PIN, false);
}

void ledOff(){
 for (uint8_t i = 0; i < LED_LINES; i++){
   ledTick(false);
 }  
 digitalWrite(LED_ENABLE_PIN, false);
}

void ledOn(){
  for (uint8_t i = 0; i < LED_LINES; i++){
    ledTick(true);
  }
  digitalWrite(LED_ENABLE_PIN, true);
}

void ledNext(){

  if(ledCounter++ > LED_LINES){
    ledTick(false);
    ledCounter = 0;
  }else{
    ledTick(true);
  }
  
}
