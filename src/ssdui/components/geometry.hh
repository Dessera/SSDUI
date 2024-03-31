#pragma once

#include "ssdui/context/component.hh"
#include "ssdui/geometry/line.hh"
#include "ssdui/geometry/point.hh"
#include "ssdui/geometry/rectangle.hh"
namespace SSDUI::Components {

template <typename Pl>
class Point : public SSDUI::Context::BaseComponent<Pl> {
 private:
  Geometry::Point<int32_t> point_;

 public:
  explicit Point(Geometry::Point<int32_t> point) : point_(point) {}
  virtual ~Point() = default;

  Point(const Point&) = delete;
  Point& operator=(const Point&) = delete;
  Point(Point&&) = delete;
  Point& operator=(Point&&) = delete;

  void operator()(SSDUI::Context::Context<Pl>* context) override {
    if (point_.x < 0 || point_.x >= context->buffer().width() || point_.y < 0 ||
        point_.y >= context->buffer().height()) {
      return;
    }
    context->buffer().mixing(point_.x, point_.y / 8, 0x01 << (point_.y % 8));
  }
};

template <typename Pl>
class Line : public SSDUI::Context::BaseComponent<Pl> {
 private:
  Geometry::Line<int32_t> line_;

 public:
  explicit Line(Geometry::Line<int32_t> line) : line_(line) {}
  virtual ~Line() = default;

  Line(const Line&) = delete;
  Line& operator=(const Line&) = delete;
  Line(Line&&) = delete;
  Line& operator=(Line&&) = delete;

  void operator()(SSDUI::Context::Context<Pl>* context) override {
    auto [start, end] = line_;
    int32_t dx = end.x - start.x;
    int32_t dy = end.y - start.y;
    int32_t x = start.x;
    int32_t y = start.y;
    int32_t x_inc = (dx < 0) ? -1 : 1;
    int32_t y_inc = (dy < 0) ? -1 : 1;
    dx = (dx < 0) ? -dx : dx;
    dy = (dy < 0) ? -dy : dy;

    if (dx > dy) {
      int32_t p = 2 * dy - dx;
      for (int32_t i = 0; i <= dx; i++) {
        // context->buffer().mixing(x, y / 8, 0x01 << (y % 8));
        Point<Pl>(Geometry::Point<int32_t>(x, y))(context);
        if (p >= 0) {
          y += y_inc;
          p -= 2 * dx;
        }
        x += x_inc;
        p += 2 * dy;
      }
    } else {
      int32_t p = 2 * dx - dy;
      for (int32_t i = 0; i <= dy; i++) {
        // context->buffer().mixing(x, y / 8, 0x01 << (y % 8));
        Point<Pl>(Geometry::Point<int32_t>(x, y))(context);
        if (p >= 0) {
          x += x_inc;
          p -= 2 * dy;
        }
        y += y_inc;
        p += 2 * dx;
      }
    }
  }
};

template <typename Pl>
class Rectangle : public SSDUI::Context::BaseComponent<Pl> {
 private:
  Geometry::Rectangle<int32_t> rectangle_;

 public:
  explicit Rectangle(Geometry::Rectangle<int32_t> rectangle)
      : rectangle_(rectangle) {}
  virtual ~Rectangle() = default;

  Rectangle(const Rectangle&) = delete;
  Rectangle& operator=(const Rectangle&) = delete;
  Rectangle(Rectangle&&) = delete;
  Rectangle& operator=(Rectangle&&) = delete;

  void operator()(SSDUI::Context::Context<Pl>* context) override {
    // solid rectangle
    auto [origin, size] = rectangle_;
    for (int32_t x = origin.x; x < origin.x + size.x; x++) {
      for (int32_t y = origin.y; y < origin.y + size.y; y++) {
        Point<Pl>(Geometry::Point<int32_t>(x, y))(context);
      }
    }
  }
};

template <typename Pl>
class Frame : public SSDUI::Context::BaseComponent<Pl> {
 private:
  Geometry::Rectangle<int32_t> rectangle_;

 public:
  explicit Frame(Geometry::Rectangle<int32_t> rectangle)
      : rectangle_(rectangle) {}
  virtual ~Frame() = default;

  Frame(const Frame&) = delete;
  Frame& operator=(const Frame&) = delete;
  Frame(Frame&&) = delete;
  Frame& operator=(Frame&&) = delete;

  void operator()(SSDUI::Context::Context<Pl>* context) override {
    // frame
    auto [origin, size] = rectangle_;
    for (int32_t x = origin.x; x < origin.x + size.x; x++) {
      Point<Pl>(Geometry::Point<int32_t>(x, origin.y))(context);
      Point<Pl>(Geometry::Point<int32_t>(x, origin.y + size.y - 1))(context);
    }
    for (int32_t y = origin.y; y < origin.y + size.y; y++) {
      Point<Pl>(Geometry::Point<int32_t>(origin.x, y))(context);
      Point<Pl>(Geometry::Point<int32_t>(origin.x + size.x - 1, y))(context);
    }
  }
};

}  // namespace SSDUI::Components