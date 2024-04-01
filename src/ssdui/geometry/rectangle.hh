#pragma once

#include "ssdui/geometry/point.hh"

namespace SSDUI::Geometry {

template <typename T>
  requires std::is_arithmetic_v<T>
struct Rectangle {
  Point<T> origin;
  Point<T> size;

  Rectangle() = default;
  Rectangle(Point<T> origin, Point<T> size) : origin(origin), size(size) {}

  Rectangle operator+(const Point<T>& rhs) const {
    return Rectangle(origin + rhs, size);
  }

  Rectangle operator-(const Point<T>& rhs) const {
    return Rectangle(origin - rhs, size);
  }

  Rectangle operator*(T rhs) const {
    return Rectangle(origin * rhs, size * rhs);
  }

  Rectangle operator/(T rhs) const {
    return Rectangle(origin / rhs, size / rhs);
  }

  Rectangle& operator+=(const Point<T>& rhs) {
    origin += rhs;
    return *this;
  }

  Rectangle& operator-=(const Point<T>& rhs) {
    origin -= rhs;
    return *this;
  }

  Rectangle& operator*=(T rhs) {
    origin *= rhs;
    return *this;
  }

  Rectangle& operator/=(T rhs) {
    origin /= rhs;
    return *this;
  }

  bool operator==(const Rectangle& rhs) const {
    return origin == rhs.origin && size == rhs.size;
  }

  bool operator!=(const Rectangle& rhs) const {
    return origin != rhs.origin || size != rhs.size;
  }
};

}  // namespace SSDUI::Geometry