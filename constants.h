#pragma once

namespace Pins {
  constexpr int8_t SCL = 4;
  constexpr int8_t SDA = 5;
}

namespace LabelOffset {
  constexpr int16_t X[2] = {30, 96};
  constexpr int16_t Y[2] = {26, 60};
  constexpr int8_t LENGTH = 4;
}

namespace BoxOffset {
  constexpr int16_t X[2] = {2, 66};
  constexpr int16_t Y[2] = {0, 34};

  constexpr int16_t WIDTH  = 60;
  constexpr int16_t HEIGHT = 30;
  constexpr int16_t RADIUS = 3;
}