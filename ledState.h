#pragma once
#include "constants.h"
static const int validateLedState = 1 << LedPins::size;

class LedState {

  int* ledStatePointer;

  public: 
    LedState(int* ledState) : ledStatePointer(ledState) {}
    
    void draw() {
      for (int i = 0; i < LedPins::size; i++) {
        digitalWrite(LedPins::pins[i], bitRead(*ledStatePointer, i));
      }
    }

    bool isValid(int state) {
      return state < validateLedState;
    }
};