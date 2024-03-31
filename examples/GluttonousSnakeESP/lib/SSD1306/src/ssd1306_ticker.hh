#pragma once

#include <cstdint>
#include <memory>
#include <thread>
#include <vector>

#include "HardwareSerial.h"
#include "esp32-hal.h"
#include "ssd1306.hh"
#include "ssd1306_command.hh"
#include "ssdui/context/context.hh"
#include "ssdui/geometry/rectangle.hh"
#include "ssdui/platform/concepts.hh"

namespace SSD1306 {

template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class Ticker {
 private:
  using SSDUIContext = SSDUI::Context::Context<Pl>;

  std::unique_ptr<SSDUIContext> context_;
  std::thread ticker_thread_;

  void _ticker() {
    auto& config = context_->config();

    // initialize the components tree
    context_->root()->on_mount(context_.get());

    while (true) {
      auto start = std::chrono::high_resolution_clock::now();

      context_->root()->operator()(context_.get());
      auto dirty_regions = _get_dirty_rectangles(context_->buffer());

      for (const auto& region : dirty_regions) {
        auto sequence = context_->buffer().next().subspan(
            region.origin.x + region.origin.y * context_->buffer().width(),
            region.size.x);

        SetColumnAddress<Pl>(region.origin.x, region.origin.x + region.size.x -
                                                  1)(context_.get());
        SetPageAddress<Pl>(region.origin.y,
                           region.origin.y + region.size.y - 1)(context_.get());

        context_->renderer()->data(sequence);
      }

      context_->buffer().swap();
      context_->buffer().clear();

      auto end = std::chrono::high_resolution_clock::now();

      auto duration =
          std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      auto frame_time = std::chrono::milliseconds(1000) / config.fps;
      if (duration < frame_time) {
        std::this_thread::sleep_for(frame_time - duration);
      }
    }
  }

  std::vector<SSDUI::Geometry::Rectangle<int32_t>> _get_dirty_rectangles(
      const SSDUI::Context::Buffer& buffer) {
    std::vector<SSDUI::Geometry::Rectangle<int32_t>> dirty_regions{};
    auto previous = buffer.prev();
    auto next = buffer.next();
    auto width = buffer.width();
    auto height = buffer.height();

    for (int32_t y = 0; y < height; y++) {
      for (int32_t x = 0; x < width; x++) {
        if (next[x + y * width] != previous[x + y * width]) {
          int32_t start = x;
          while (x < width && next[x + y * width] != previous[x + y * width]) {
            x++;
          }
          dirty_regions.emplace_back(
              SSDUI::Geometry::Rectangle<int32_t>{{start, y}, {x - start, 1}});
        }
      }
    }

    return dirty_regions;
  }

 public:
  explicit Ticker(std::unique_ptr<SSDUIContext> context)
      : context_(std::move(context)), ticker_thread_([this] { _ticker(); }) {}

  ~Ticker() {
    if (ticker_thread_.joinable()) {
      ticker_thread_.join();
    }
  }
};

}  // namespace SSD1306
