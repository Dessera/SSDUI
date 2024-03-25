#pragma once

#include <cstdint>
namespace ssdui::geometry {

struct Point {
  std::uint8_t x;
  std::uint8_t y;
} __attribute__((aligned(2)));

struct Rect {
  Point origin;
  std::uint8_t width;
  std::uint8_t height;
} __attribute__((aligned(4)));

}  // namespace ssdui::geometry
