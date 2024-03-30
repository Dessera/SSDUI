#pragma once

#include "ssdui/geometry/point.hh"
namespace SSDUI::Geometry {

template <typename T>
  requires std::is_arithmetic_v<T>
struct Line {
  Point<T> start;
  Point<T> end;

  Line() = default;
  Line(Point<T> start, Point<T> end) : start(start), end(end) {}

  Line operator+(const Point<T>& rhs) const {
    return Line(start + rhs, end + rhs);
  }

  Line operator-(const Point<T>& rhs) const {
    return Line(start - rhs, end - rhs);
  }

  Line operator*(T rhs) const { return Line(start * rhs, end * rhs); }

  Line operator/(T rhs) const { return Line(start / rhs, end / rhs); }

  Line& operator+=(const Point<T>& rhs) {
    start += rhs;
    end += rhs;
    return *this;
  }

  Line& operator-=(const Point<T>& rhs) {
    start -= rhs;
    end -= rhs;
    return *this;
  }

  Line& operator*=(T rhs) {
    start *= rhs;
    end *= rhs;
    return *this;
  }

  Line& operator/=(T rhs) {
    start /= rhs;
    end /= rhs;
    return *this;
  }

  bool operator==(const Line& rhs) const {
    return start == rhs.start && end == rhs.end;
  }

  bool operator!=(const Line& rhs) const {
    return start != rhs.start || end != rhs.end;
  }
};

}  // namespace SSDUI::Geometry