#pragma once

#include <cstdint>
#include <vector>

#include "ssdui/common/geometry.hh"
#include "ssdui/common/span.hh"

namespace ssdui::context {

class CachedBuffer {
 private:
  common::Span<uint8_t> m_current;
  common::Span<uint8_t> m_previous;

  uint8_t m_width;
  uint8_t m_page;

 public:
  CachedBuffer(uint8_t width, uint8_t page)
      : m_width(width),
        m_page(page),
        m_current(new uint8_t[width * page](), width * page),
        m_previous(new uint8_t[width * page](), width * page) {
    std::fill(m_current.begin(), m_current.end(), 0xFF);
    std::fill(m_previous.begin(), m_previous.end(), 0x00);
  }

  ~CachedBuffer() {
    delete[] m_current.data();
    delete[] m_previous.data();
  }

  CachedBuffer(const CachedBuffer&) = delete;
  CachedBuffer& operator=(const CachedBuffer&) = delete;
  CachedBuffer(CachedBuffer&&) = delete;
  CachedBuffer& operator=(CachedBuffer&&) = delete;

  uint8_t& operator()(uint8_t x, uint8_t y) {
    return m_current[x + y * m_width];
  }

  void swap() noexcept { std::swap(m_current, m_previous); }
  void clear() { std::fill(m_current.begin(), m_current.end(), 0); }

  std::vector<geometry::Rect> get_dirty_regions();
  common::Span<uint8_t> get_area(const geometry::Rect& rect);
};

}  // namespace ssdui::context
