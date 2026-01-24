#pragma once

#include "label.h"
#include "types.h"
#include "constants.h"

#define DISPLAY_CONTRAST 128;

U8G2_SSD1309_128X64_NONAME2_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, /* clock=*/ Pins::SCL, /* data=*/ Pins::SDA);

class Display {
  const RBox rBoxes[LabelOffset::LENGTH] = {
    {BoxOffset::X[0], BoxOffset::Y[0]},
    {BoxOffset::X[1], BoxOffset::Y[0]},
    {BoxOffset::X[0], BoxOffset::Y[1]},
    {BoxOffset::X[1], BoxOffset::Y[1]}
  };

  void drawRBox(const RBox& box) {
    u8g2.drawRBox(box.x, box.y, box.width, box.height, box.radius);
  }

  public:
    void setup(){
      u8g2.begin();
      u8g2.enableUTF8Print();
      u8g2.setContrast(DISPLAY_CONTRAST);
      u8g2.setFontMode(1);  // テキストの背景を透明に設定
      u8g2.setFont(u8g2_font_b12_b_t_japanese3);
    }

    void updateDisplay(int8_t ledState, Label labels[]){
      u8g2.clearBuffer();
      for(int8_t i=0; i<LabelOffset::LENGTH; i++) {
        if(bitRead(ledState, i)) {
          u8g2.setDrawColor(1);
          drawRBox(rBoxes[i]);
        }
        u8g2.setDrawColor(!bitRead(ledState, i));
        labels[i].draw(u8g2);
      }
      u8g2.sendBuffer();
    }
};
