#include <U8g2lib.h>
#include "serverClient.h"
#include "display.h"
#include "constants.h"
#include "label.h"

unsigned long now;
unsigned long timer100ms = 0;

int8_t ledState = 0;

Label labels[LabelOffset::LENGTH] = {
  {"会議中", LabelOffset::X[0], LabelOffset::Y[0]},
  {"腹減った", LabelOffset::X[1], LabelOffset::Y[0]},
  {"疲れた", LabelOffset::X[0], LabelOffset::Y[1]},
  {"休憩中", LabelOffset::X[1], LabelOffset::Y[1]}
};

ServerClient serverClient(&ledState);
Display display;

void handle100ms() {
  if (now - timer100ms > 100) {
    // 100ms単位で動く処理
    display.printLcd(ledState, labels);
    timer100ms = now;
  }
}

void setup(void) {
  Serial.begin(115200);
  serverClient.setup();
  display.setup();
}

void loop(void) {
  now = millis();
  handle100ms();
  serverClient.setloop();
}
