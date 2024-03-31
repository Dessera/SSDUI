#pragma once

#include <cstdint>
namespace SSD1306 {

/**
 * @brief 内存寻址模式
 */
enum class AddressMode : uint8_t {
  HORIZONTAL = 0x00,
  VERTICAL = 0x01,
  PAGE = 0x02,
};

/**
 * @brief 硬件接口配置
 */
enum class ComPinsConfig : uint8_t {
  SEQUENTIAL = 0x02,
  SEQUENTIAL_REMAPPED = 0x22,
  ALTERNATIVE = 0x12,
  ALTERNATIVE_REMAPPED = 0x32,
};

struct Config {
  static constexpr int16_t DEFAULT_WIDTH = 128;
  static constexpr int16_t DEFAULT_HEIGHT = 64;
  static constexpr AddressMode DEFAULT_ADDRESSING_MODE = AddressMode::PAGE;
  static constexpr uint8_t DEFAULT_RATIO = 0x00;
  static constexpr uint8_t DEFAULT_FREQUENCY = 0x08;
  static constexpr uint8_t DEFAULT_PRECHARGE_PHASE1 = 0x01;
  static constexpr uint8_t DEFAULT_PRECHARGE_PHASE2 = 0x0F;
  static constexpr uint8_t DEFAULT_VCOMH_LEVEL = 0x40;
  static constexpr uint8_t DEFAULT_CONTRAST = 0x7F;
  static constexpr ComPinsConfig DEFAULT_COM_PINS = ComPinsConfig::ALTERNATIVE;
  static constexpr bool DEFAULT_ENTIRE_DISPLAY_ON = false;
  static constexpr bool DEFAULT_INVERSE_DISPLAY = false;
  static constexpr bool DEFAULT_DISPLAY_ON = true;
  static constexpr uint8_t DEFAULT_START_LINE = 0;
  static constexpr bool DEFAULT_HORIZONTAL_FLIP = false;
  static constexpr bool DEFAULT_VERTICAL_FLIP = false;
  static constexpr uint8_t DEFAULT_DISPLAY_OFFSET = 0;
  static constexpr uint8_t DEFAULT_MULTIPLEX_RATIO = 63;
  static constexpr bool DEFAULT_CHARGE_PUMP_ENABLE = true;
  static constexpr int16_t DEFAULT_FRAME_RATE = 30;

  int16_t width{DEFAULT_WIDTH};
  int16_t height{DEFAULT_HEIGHT};
  AddressMode addressing_mode{DEFAULT_ADDRESSING_MODE};
  uint8_t clock_ratio{DEFAULT_RATIO};
  uint8_t clock_frequency{DEFAULT_FREQUENCY};
  uint8_t precharge_phase1{DEFAULT_PRECHARGE_PHASE1};
  uint8_t precharge_phase2{DEFAULT_PRECHARGE_PHASE2};
  uint8_t vcomh_level{DEFAULT_VCOMH_LEVEL};
  uint8_t contrast{DEFAULT_CONTRAST};
  bool entire_display_on{DEFAULT_ENTIRE_DISPLAY_ON};
  bool inverse_display{DEFAULT_INVERSE_DISPLAY};
  bool display_on{DEFAULT_DISPLAY_ON};
  uint8_t start_line{DEFAULT_START_LINE};
  bool horizontal_flip{DEFAULT_HORIZONTAL_FLIP};
  bool vertical_flip{DEFAULT_VERTICAL_FLIP};
  uint8_t multiplex_ratio{DEFAULT_MULTIPLEX_RATIO};
  uint8_t display_offset{DEFAULT_DISPLAY_OFFSET};
  ComPinsConfig com_pins{DEFAULT_COM_PINS};
  bool charge_pump_enable{DEFAULT_CHARGE_PUMP_ENABLE};
  int16_t fps{DEFAULT_FRAME_RATE};
} __attribute__((aligned(32)));

}  // namespace SSD1306