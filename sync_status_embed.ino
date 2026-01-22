#include <U8g2lib.h>
#include <EEPROM.h>
#include "serverClient.h"
#include "display.h"
#include "constants.h"
#include "label.h"

unsigned long now;
unsigned long timer100ms = 0;

int8_t ledState = 0;

Label labels[LabelOffset::LENGTH] = {
  {LabelOffset::X[0], LabelOffset::Y[0]},
  {LabelOffset::X[1], LabelOffset::Y[0]},
  {LabelOffset::X[0], LabelOffset::Y[1]},
  {LabelOffset::X[1], LabelOffset::Y[1]}
};

ServerClient serverClient(&ledState, labels);
Display display;

void handle100ms() {
  if (now - timer100ms > 100) {
    // 100ms単位で動く処理
    display.printLcd(ledState, labels);
    timer100ms = now;
  }
}

void setup(void) {
  EEPROM.begin(512);
  for(int8_t i=0; i<LabelOffset::LENGTH; i++) {
    String defaultText = "ラベル" + String(i+1);
    labels[i].begin(defaultText.c_str());
  }
  Serial.begin(115200);
  serverClient.setup();
  display.setup();
}

void loop(void) {
  now = millis();
  handle100ms();
  serverClient.setloop();
}
