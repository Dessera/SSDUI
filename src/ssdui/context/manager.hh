#pragma once

#include "ssdui/component/address.hh"

namespace ssdui::context {

template <typename Rnd>
class GlobalContextManager {
 private:
  std::unique_ptr<Context<Rnd>> m_ctx;
  std::thread m_update_handler;

  void _update_thread_handler() {
    CachedBuffer &buffer = m_ctx->buffer();
    Rnd *renderer = m_ctx->renderer();

    while (true) {
      // current time stamp
      auto start = std::chrono::high_resolution_clock::now();

      // execute root component
      m_ctx->root()->operator()(m_ctx.get());

      auto dirty_regions = buffer.get_dirty_regions();
      // render dirty regions
      for (const auto &region : dirty_regions) {
        auto [origin, width, height] = region;
        auto area = buffer.get_area(region);
        // set position
        command::address::SetColumnAddress<Rnd>(
            origin.x, origin.x + width - 1)(m_ctx.get());
        command::address::SetPageAddress<Rnd>(
            origin.y, origin.y + height - 1)(m_ctx.get());
        // render
        renderer->data(area);
      }

      // update previous buffer
      buffer.swap();
      buffer.clear();

      auto end = std::chrono::high_resolution_clock::now();

      // sleep for the rest of the frame (30fps)
      auto duration =
          std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      std::this_thread::sleep_for(std::chrono::milliseconds(33) - duration);
    }
  }

 public:
  explicit GlobalContextManager(std::unique_ptr<Rnd> &&ctx)
      : m_ctx(std::move(ctx)) {
    m_update_handler =
        std::thread([this]() { this->_update_thread_handler(); });
  }
  ~GlobalContextManager() { m_update_handler.join(); }
};

}  // namespace ssdui::context
