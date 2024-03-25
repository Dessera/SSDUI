#include "ssdui/context/buffer.hh"

#include <cstddef>
#include <cstdint>
#include <vector>

#include "ssdui/common/geometry.hh"

namespace ssdui::context {

std::vector<geometry::Rect> CachedBuffer::get_dirty_regions() {
  // 在每个page中找到连续的dirty区域
  std::vector<geometry::Rect> dirty_regions;

  for (std::size_t y = 0; y < m_page; y++) {
    std::size_t x = 0;
    while (x < m_width) {
      if (m_current[x + y * m_width] != m_previous[x + y * m_width]) {
        std::size_t start = x;
        while (x < m_width &&
               m_current[x + y * m_width] != m_previous[x + y * m_width]) {
          x++;
        }
        dirty_regions.emplace_back(geometry::Rect{{start, y}, x - start, 1});
      } else {
        x++;
      }
    }
  }

  return dirty_regions;
}

common::Span<uint8_t> CachedBuffer::get_area(const geometry::Rect& rect) {
  auto [origin, width, height] = rect;
  return {m_current.data() + origin.x + origin.y * m_width, width * height};
}

}  // namespace ssdui::context
