//////////////////  PINS  /////////////////////////////

#define LED_DATA_PIN                8
#define LED_CLOCK_PIN               9
#define LED_ENABLE_PIN             10

/////////////////// LEDs //////////////////////////
#define NUM_LED_LINES              18

/////////////////// global vars //////////////////
uint8_t     ledCounter = 0;

#define SPEED                      1200
#define LINES                      3
#define SPACES                     3

void setup() {
  pinMode(13, OUTPUT);
  pinMode(LED_DATA_PIN,   OUTPUT);
  pinMode(LED_CLOCK_PIN,  OUTPUT);
  pinMode(LED_ENABLE_PIN, OUTPUT);
  
  ledAll(false);
  digitalWrite(LED_ENABLE_PIN, true);
}

void loop() {
   for (int i=0; i < SPACES; i++){
      ledTick(false);
      delay(SPEED);
   }
   for (int i=0; i < LINES; i++){
      ledTick(true);
      delay(SPEED);
   }
}

void ledLines() {
  digitalWrite(LED_DATA_PIN, HIGH);
  digitalWrite(LED_CLOCK_PIN, true);
  delay(10);  //TODO check if neccessary
  digitalWrite(LED_CLOCK_PIN, false);
}

void ledTick(bool onoff){
  digitalWrite(LED_DATA_PIN, onoff);
  digitalWrite(LED_CLOCK_PIN, true);
  delay(10);  //TODO check if neccessary
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
