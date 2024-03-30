#pragma once

#include <type_traits>
namespace SSDUI::Geometry {

template <typename T>
  requires std::is_arithmetic_v<T>
struct Point {
  T x;
  T y;

  Point() = default;
  Point(T x, T y) : x(x), y(y) {}

  Point operator+(const Point& rhs) const {
    return Point(x + rhs.x, y + rhs.y);
  }

  Point operator-(const Point& rhs) const {
    return Point(x - rhs.x, y - rhs.y);
  }

  Point operator*(T rhs) const { return Point(x * rhs, y * rhs); }

  Point operator/(T rhs) const { return Point(x / rhs, y / rhs); }

  Point& operator+=(const Point& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  Point& operator-=(const Point& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }

  Point& operator*=(T rhs) {
    x *= rhs;
    y *= rhs;
    return *this;
  }

  Point& operator/=(T rhs) {
    x /= rhs;
    y /= rhs;
    return *this;
  }

  bool operator==(const Point& rhs) const { return x == rhs.x && y == rhs.y; }

  bool operator!=(const Point& rhs) const { return x != rhs.x || y != rhs.y; }
};

}  // namespace SSDUI::Geometry