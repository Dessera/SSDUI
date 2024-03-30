#pragma once

#include <cstdint>
#include <mutex>
#include <ssdui/common/span.hh>
#include <ssdui/platform/concepts.hh>

#include "Wire.h"
namespace SSDUI::Platform {

/**
 * @brief 测试用的 SSD1306 平台
 */
class SSD1306 {
 public:
  class Renderer {
   public:
    static constexpr uint8_t COMMAND_PREFIX = 0x00;
    static constexpr uint8_t DATA_PREFIX = 0x40;

   private:
    TwoWire* wire_;
    uint8_t address_;

    uint8_t sda_;
    uint8_t scl_;

    uint32_t frequency_;

    std::mutex mtx_{};

   public:
    Renderer(TwoWire* wire, uint8_t address, uint8_t sda, uint8_t scl,
             uint32_t frequency);
    Renderer(TwoWire* wire, uint8_t address, uint8_t sda, uint8_t scl);
    Renderer(TwoWire* wire, uint8_t address);

    ~Renderer();

    std::size_t command(std::span<uint8_t> data);
    std::size_t data(std::span<uint8_t> data);
  };

  struct Config {
    std::size_t width{128};
    std::size_t height{64};
    std::size_t fps{30};
    bool vertical_flip{false};
    bool horizontal_flip{false};

    // (dessera) 我决定不实现更多的硬件配置项，因为这个平台只是一个测试用的平台

  } __attribute__((aligned(16)));
};

static_assert(IsPlatform<SSD1306>, "TestBench SSD1306 is not a platform");

}  // namespace SSDUI::Platform