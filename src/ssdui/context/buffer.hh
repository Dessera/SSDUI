#pragma once

#include <cstdint>
#include <vector>

#include "ssdui/common/geometry.hh"
#include "ssdui/common/span.hh"

namespace ssdui::context {

class CachedBuffer {
 private:
  std::vector<uint8_t> m_current;
  std::vector<uint8_t> m_previous;

  uint8_t m_width;
  uint8_t m_page;

 public:
  CachedBuffer(uint8_t width, uint8_t page)
      : m_width(width),
        m_page(page),
        m_current(width * page, 0xFF),
        m_previous(width * page, 0x00) {}

  uint8_t& operator()(uint8_t x, uint8_t y) {
    return m_current[x + y * m_width];
  }

  void swap() noexcept { std::swap(m_current, m_previous); }
  void clear() { std::fill(m_current.begin(), m_current.end(), 0); }

  std::vector<geometry::Rect> get_dirty_regions();
  common::Span<uint8_t> get_area(const geometry::Rect& rect);
};

}  // namespace ssdui::context
