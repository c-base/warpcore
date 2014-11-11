#include <Wtv020sd16p.h>

#define RESET_PIN 2
#define CLOCK_PIN 3
#define DATA_PIN  4
#define BUSY_PIN  5

Wtv020sd16p wtv020sd16p(RESET_PIN, CLOCK_PIN, DATA_PIN, BUSY_PIN);

void setup() {
  wtv020sd16p.reset();
}

void loop() {
  wtv020sd16p.playVoice(0);
  wtv020sd16p.asyncPlayVoice(1);
  delay(5000);
  wtv020sd16p.pauseVoice();
  delay(5000);
  wtv020sd16p.pauseVoice();
  delay(5000);  
  wtv020sd16p.stopVoice();
}


