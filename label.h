#pragma once

#include <EEPROM.h>

class Label {
  static constexpr uint8_t TEXT_LEN = 20;  // UTF-8 日本語6文字 + '\0'

  const int16_t x;
  const int16_t y;
  const uint16_t eepromAddr;

  char text[TEXT_LEN];

  static uint16_t allocateAddr() {
    static uint16_t nextAddr = 0;
    uint16_t addr = nextAddr;
    nextAddr += TEXT_LEN;
    return addr;
  }

  void load() {
    for (uint8_t i = 0; i < TEXT_LEN; i++) {
      text[i] = EEPROM.read(eepromAddr + i);
      if (text[i] == '\0') break;
    }
  }

  void save() {
    for (uint8_t i = 0; i < TEXT_LEN; i++) {
      uint8_t v = text[i];
      if (EEPROM.read(eepromAddr + i) != v) {
        EEPROM.write(eepromAddr + i, v);
      }
      if (text[i] == '\0') break;
    }
    EEPROM.commit();
  }

  bool isValid() const {
    if (text[0] == '\0') return false;
    if (text[0] == (char)0xFF) return false;
    return true;
  }

public:
  Label(int16_t x_, int16_t y_)
    : x(x_), y(y_), eepromAddr(allocateAddr()) {
    text[0] = '\0';
  }

  void begin(const char* defaultText) {
    load();
    if (!isValid()) {
      setText(defaultText);
    }
  }

  const char* getText() const {
    return text;
  }

  void setText(const char* t) {
    if (strncmp(text, t, TEXT_LEN) == 0) return;

    strncpy(text, t, TEXT_LEN);
    text[TEXT_LEN - 1] = '\0';
    save();
  }

  void draw(U8G2& u8g2) const {
    uint16_t w = u8g2.getUTF8Width(text);
    int16_t drawX = x - (w / 2);
    u8g2.drawUTF8(drawX, y, text);
  }
};
