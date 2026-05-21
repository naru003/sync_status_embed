#include "serverClient.h"
#include "constants.h"

int ledState = 0;

ServerClient serverClient(&ledState);

void setup(void) {
  Serial.begin(115200);
  serverClient.setup();

  for (int i = 0; i < LedPins::size; i++) {
    pinMode(LedPins::pins[i], OUTPUT);
  }
}

void loop(void) {
  serverClient.setloop();
}
