#pragma once

#include "constants.h"

struct RBox {
  int16_t x;
  int16_t y;
  int16_t width;
  int16_t height;
  int16_t radius;

  constexpr RBox(
    int16_t x_,
    int16_t y_,
    int16_t w_ = BoxOffset::WIDTH,
    int16_t h_ = BoxOffset::HEIGHT,
    int16_t r_ = BoxOffset::RADIUS
  )
    : x(x_), y(y_), width(w_), height(h_), radius(r_) {}

};