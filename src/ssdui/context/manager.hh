#pragma once

#include <thread>

#include "ssdui/component/address.hh"
#include "ssdui/context/config.hh"
#include "ssdui/context/core.hh"

namespace ssdui::context {

/**
 * @brief 全局上下文管理器
 *
 *        用于容纳全局上下文，负责真正的渲染调度
 *        不允许任何拷贝或移动操作，保证全局上下文的唯一性
 */
template <typename Rnd>
class GlobalContextManager {
 private:
  std::unique_ptr<Context<Rnd>> m_ctx;
  std::thread m_update_handler;

  /**
   * @brief 更新线程处理函数
   *
   *        为了简化算法，只合并同一个 page 内的 dirty region
   */
  void _update_thread_handler() {
    auto &buffer = m_ctx->buffer();
    Rnd *renderer = m_ctx->renderer();
    RuntimeConfig &config = m_ctx->config();

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

      // sleep for the rest of the frame time (from config)
      auto duration =
          std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      auto frame_time = std::chrono::milliseconds(1000 / config.frame_rate());
      if (duration < frame_time) {
        std::this_thread::sleep_for(frame_time - duration);
      }
    }
  }

 public:
  explicit GlobalContextManager(std::unique_ptr<Context<Rnd>> &&ctx)
      : m_ctx(std::move(ctx)),
        m_update_handler([this]() { this->_update_thread_handler(); }) {}
  ~GlobalContextManager() { m_update_handler.join(); }

  GlobalContextManager(const GlobalContextManager &) = delete;
  GlobalContextManager &operator=(const GlobalContextManager &) = delete;
  GlobalContextManager(GlobalContextManager &&) = delete;
  GlobalContextManager &operator=(GlobalContextManager &&) = delete;
};

}  // namespace ssdui::context
