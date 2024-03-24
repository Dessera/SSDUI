#pragma once

#include <cstddef>
namespace ssdui::geometry {

struct Point {
  std::size_t x;
  std::size_t y;
} __attribute__((aligned(8)));

struct Rect {
  Point origin;
  std::size_t width;
  std::size_t height;
} __attribute__((aligned(16)));

}  // namespace ssdui::geometry
