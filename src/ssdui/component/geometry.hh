#pragma once

#include <cmath>

#include "ssdui/common/geometry.hh"
#include "ssdui/context/config.hh"
#include "ssdui/context/core.hh"
namespace ssdui::component::geometry {

template <typename Rnd>
class Point {
 public:
  using RawPoint = ssdui::geometry::Point;

 private:
  RawPoint m_point;

 public:
  explicit Point(const RawPoint& point) : m_point(point) {}

  void operator()(context::Context<Rnd>* ctx) const {
    auto& buffer = ctx->buffer();
    auto& config = ctx->config();

    if (m_point.x >= config.width() || m_point.y >= config.height()) {
      return;
    }

    auto current_page = m_point.y / context::Config::DEFAULT_PAGE_SIZE;
    buffer(m_point.x, current_page) |=
        1U << (m_point.y % context::Config::DEFAULT_PAGE_SIZE);
  }
};

template <typename Rnd>
class Line {
 public:
  using RawPoint = ssdui::geometry::Point;

 private:
  RawPoint m_start;
  RawPoint m_end;

 public:
  Line(const RawPoint& start, const RawPoint& end)
      : m_start(start), m_end(end) {}
  Line(const RawPoint& start, double angle, uint8_t length)
      : m_start(start),
        m_end({static_cast<uint8_t>(start.x + length * std::cos(angle)),
               static_cast<uint8_t>(start.y + length * std::sin(angle))}) {}

  void operator()(context::Context<Rnd>* ctx) const {
    const auto& config = ctx->config();

    if (m_start.x >= config.width() || m_start.y >= config.height() ||
        m_end.x >= config.width() || m_end.y >= config.height()) {
      return;
    }

    // fill the path with Point Class

    // Bresenham's line algorithm
    auto [x0, y0] = m_start;
    auto [x1, y1] = m_end;

    auto dx = std::abs(x1 - x0);
    auto dy = -std::abs(y1 - y0);

    auto sx = x0 < x1 ? 1 : -1;
    auto sy = y0 < y1 ? 1 : -1;

    auto err = dx + dy;

    while (true) {
      Point<Rnd>({x0, y0})(ctx);

      if (x0 == x1 && y0 == y1) {
        break;
      }

      auto e2 = 2 * err;
      if (e2 >= dy) {
        err += dy;
        x0 += sx;
      }
      if (e2 <= dx) {
        err += dx;
        y0 += sy;
      }
    }
  }
};

template <typename Rnd>
class LineFrame {
 private:
  ::ssdui::geometry::Rect m_rect;

 public:
  explicit LineFrame(const ::ssdui::geometry::Rect& rect) : m_rect(rect) {}

  void operator()(context::Context<Rnd>* ctx) const {
    Line<Rnd>({m_rect.origin.x, m_rect.origin.y},
              {m_rect.origin.x + m_rect.width, m_rect.origin.y})(ctx);
    Line<Rnd>({m_rect.origin.x, m_rect.origin.y},
              {m_rect.origin.x, m_rect.origin.y + m_rect.height})(ctx);
    Line<Rnd>(
        {m_rect.origin.x + m_rect.width, m_rect.origin.y},
        {m_rect.origin.x + m_rect.width, m_rect.origin.y + m_rect.height})(ctx);
    Line<Rnd>(
        {m_rect.origin.x, m_rect.origin.y + m_rect.height},
        {m_rect.origin.x + m_rect.width, m_rect.origin.y + m_rect.height})(ctx);
  }
};

template <typename Rnd>
class Rectangle {
 private:
  ::ssdui::geometry::Rect m_rect;

 public:
  explicit Rectangle(const ::ssdui::geometry::Rect& rect) : m_rect(rect) {}

  void operator()(context::Context<Rnd>* ctx) const {
    for (uint8_t i = 0; i < m_rect.width; ++i) {
      for (uint8_t j = 0; j < m_rect.height; ++j) {
        Point<Rnd>({static_cast<uint8_t>(m_rect.origin.x + i),
                    static_cast<uint8_t>(m_rect.origin.y + j)})(ctx);
      }
    }
  }
};

template <typename Rnd>
class Circle {
 private:
  ::ssdui::geometry::Point m_center;
  uint8_t m_radius;

 public:
  Circle(const ::ssdui::geometry::Point& center, uint8_t radius)
      : m_center(center), m_radius(radius) {}

  void operator()(context::Context<Rnd>* ctx) const {
    auto& config = ctx->config();

    if (m_center.x >= config.width() || m_center.y >= config.height()) {
      return;
    }

    auto x = m_radius;
    auto y = 0;
    auto err = 0;

    while (x >= y) {
      Point<Rnd>({static_cast<uint8_t>(m_center.x + x),
                  static_cast<uint8_t>(m_center.y + y)})(ctx);
      Point<Rnd>({static_cast<uint8_t>(m_center.x + y),
                  static_cast<uint8_t>(m_center.y + x)})(ctx);
      Point<Rnd>({static_cast<uint8_t>(m_center.x - y),
                  static_cast<uint8_t>(m_center.y + x)})(ctx);
      Point<Rnd>({static_cast<uint8_t>(m_center.x - x),
                  static_cast<uint8_t>(m_center.y + y)})(ctx);
      Point<Rnd>({static_cast<uint8_t>(m_center.x - x),
                  static_cast<uint8_t>(m_center.y - y)})(ctx);
      Point<Rnd>({static_cast<uint8_t>(m_center.x - y),
                  static_cast<uint8_t>(m_center.y - x)})(ctx);
      Point<Rnd>({static_cast<uint8_t>(m_center.x + y),
                  static_cast<uint8_t>(m_center.y - x)})(ctx);
      Point<Rnd>({static_cast<uint8_t>(m_center.x + x),
                  static_cast<uint8_t>(m_center.y - y)})(ctx);

      if (err <= 0) {
        y += 1;
        err += 2 * y + 1;
      }

      if (err > 0) {
        x -= 1;
        err -= 2 * x + 1;
      }
    }
  }
};

}  // namespace ssdui::component::geometry
