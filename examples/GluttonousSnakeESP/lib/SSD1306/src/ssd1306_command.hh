#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "esp32-hal.h"
#include "ssd1306.hh"
#include "ssdui/context/component.hh"
#include "ssdui/platform/concepts.hh"
namespace SSD1306 {

/**
 * @brief 设置起始列，用于页寻址模式
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class SetStartColumn {
 public:
  static inline constexpr std::byte COMMAND_SET_START_COLUMN_LOW{0x00};
  static inline constexpr std::byte COMMAND_SET_START_COLUMN_HIGH{0x10};
  static inline constexpr std::byte COLUMN_MASK{0x0F};

 private:
  int16_t m_column;

 public:
  explicit SetStartColumn(int16_t column) : m_column(column) {}

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    // context::RuntimeConfig& config = ctx->config();
    auto& config = ctx->config();

    if (config.addressing_mode != AddressMode::PAGE) {
      return;
    }

    if (m_column >= config.width) {
      return;
    }

    auto data = std::array<std::byte, 4>(
        {COMMAND_SET_START_COLUMN_LOW,
         static_cast<std::byte>(m_column) & COLUMN_MASK,
         COMMAND_SET_START_COLUMN_HIGH, static_cast<std::byte>(m_column >> 4)});
    ctx->renderer()->command(data);
  }
};

/**
 * @brief 设置起始页，用于页寻址模式
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class SetStartPage {
 public:
  static inline constexpr std::byte COMMAND_SET_START_PAGE{0xB0};

 private:
  uint8_t m_page;

 public:
  explicit SetStartPage(uint8_t page) : m_page(page) {}

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    auto& config = ctx->config();

    if (config.addressing_mode != AddressMode::PAGE) {
      return;
    }

    // TODO(dessera): 页大小统一指定
    if (m_page >= config.height / 8) {
      return;
    }

    auto data = std::array<std::byte, 2>(
        {COMMAND_SET_START_PAGE, static_cast<std::byte>(m_page)});
    ctx->renderer()->command(data);
  }
};

/**
 * @brief 设置寻址模式
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class SetAddressingMode {
 public:
  static inline constexpr std::byte COMMAND_SET_ADDRESSING_MODE{0x20};

 private:
  AddressMode m_mode;

 public:
  explicit SetAddressingMode(AddressMode mode) : m_mode(mode) {}

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    auto data = std::array<std::byte, 2>(
        {COMMAND_SET_ADDRESSING_MODE, static_cast<std::byte>(m_mode)});
    ctx->renderer()->command(data);
  }
};

/**
 * @brief 设置列地址范围，用于水平寻址模式和垂直寻址模式
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class SetColumnAddress {
 public:
  static inline constexpr std::byte COMMAND_SET_COLUMN_ADDRESS{0x21};

 private:
  int16_t m_start_column;
  int16_t m_end_column;

 public:
  explicit SetColumnAddress(int16_t start_column, int16_t end_column)
      : m_start_column(start_column), m_end_column(end_column) {}

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    auto& config = ctx->config();

    if (config.addressing_mode == AddressMode::PAGE) {
      // downcast to SetStartColumn
      SetStartColumn<Pl>(m_start_column).operator()(ctx);
    }

    if (m_start_column >= config.width || m_end_column >= config.width ||
        m_start_column > m_end_column || m_start_column < 0 ||
        m_end_column < 0) {
      return;
    }

    auto data = std::array<std::byte, 3>(
        {COMMAND_SET_COLUMN_ADDRESS, static_cast<std::byte>(m_start_column),
         static_cast<std::byte>(m_end_column)});
    ctx->renderer()->command(data);
  }
};

/**
 * @brief 设置页地址范围，用于水平寻址模式和垂直寻址模式
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class SetPageAddress {
 public:
  static inline constexpr std::byte COMMAND_SET_PAGE_ADDRESS{0x22};

 private:
  int16_t m_start_page;
  int16_t m_end_page;

 public:
  explicit SetPageAddress(int16_t start_page, int16_t end_page)
      : m_start_page(start_page), m_end_page(end_page) {}

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    auto& config = ctx->config();

    if (config.addressing_mode == AddressMode::PAGE) {
      // downcast to SetStartPage
      SetStartPage<Pl>(m_start_page).operator()(ctx);
    }

    // TODO(dessera): 页大小统一指定
    if (m_start_page >= config.height / 8 || m_end_page >= config.height / 8 ||
        m_start_page > m_end_page || m_start_page < 0 || m_end_page < 0) {
      return;
    }

    auto data = std::array<std::byte, 3>({COMMAND_SET_PAGE_ADDRESS,
                                          static_cast<std::byte>(m_start_page),
                                          static_cast<std::byte>(m_end_page)});
    ctx->renderer()->command(data);
  }
};

/**
 * @brief 什么都不做
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class Nop {
 public:
  static inline constexpr std::byte COMMAND_CODE{0xE3};

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    auto data = std::array<std::byte, 1>({COMMAND_CODE});
    ctx->renderer()->command(data);
  }
};

/**
 * @brief 设置显示开关
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class SetDisplay {
 public:
  static inline constexpr std::byte COMMAND_DISPLAY_ON{0xAF};
  static inline constexpr std::byte COMMAND_DISPLAY_OFF{0xAE};

 private:
  bool m_state;

 public:
  explicit SetDisplay(bool state) : m_state(state) {}

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    auto data = std::array<std::byte, 1>(
        {m_state ? COMMAND_DISPLAY_ON : COMMAND_DISPLAY_OFF});
    ctx->renderer()->command(data);
  }
};

/**
 * @brief 设置对比度
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class SetContrast {
 public:
  static inline constexpr std::byte COMMAND_SET_CONTRAST{0x81};

 private:
  uint8_t m_contrast;

 public:
  explicit SetContrast(uint8_t contrast) : m_contrast(contrast) {}

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    auto data = std::array<std::byte, 2>(
        {COMMAND_SET_CONTRAST, static_cast<std::byte>(m_contrast)});
    ctx->renderer()->command(data);
  }
};

/**
 * @brief 设置显示反转
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class SetInvert {
 public:
  static inline constexpr std::byte COMMAND_INVERT_ON{0xA7};
  static inline constexpr std::byte COMMAND_INVERT_OFF{0xA6};

 private:
  bool m_state;

 public:
  explicit SetInvert(bool state) : m_state(state) {}

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    auto data = std::array<std::byte, 1>(
        {m_state ? COMMAND_INVERT_ON : COMMAND_INVERT_OFF});
    ctx->renderer()->command(data);
  }
};

/**
 * @brief 设置显示全亮
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class SetEntireDisplay {
 public:
  static inline constexpr std::byte COMMAND_ENTIRE_DISPLAY_ON{0xA5};
  static inline constexpr std::byte COMMAND_ENTIRE_DISPLAY_OFF{0xA4};

 private:
  bool m_state;

 public:
  explicit SetEntireDisplay(bool state) : m_state(state) {}

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    auto data = std::array<std::byte, 1>(
        {m_state ? COMMAND_ENTIRE_DISPLAY_ON : COMMAND_ENTIRE_DISPLAY_OFF});
    ctx->renderer()->command(data);
  }
};

/**
 * @brief 设置预充电周期
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class SetPrechargePeriod {
 public:
  static inline constexpr std::byte COMMAND_SET_PRECHARGE_PERIOD{0xD9};

 private:
  uint8_t m_period_phase1;
  uint8_t m_period_phase2;

 public:
  explicit SetPrechargePeriod(uint8_t period_phase1, uint8_t period_phase2)
      : m_period_phase1(period_phase1), m_period_phase2(period_phase2) {}

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    auto data = std::array<std::byte, 2>(
        {COMMAND_SET_PRECHARGE_PERIOD,
         static_cast<std::byte>((m_period_phase2 << 4) | m_period_phase1)});
    ctx->renderer()->command(data);
  }
};

/**
 * @brief 设置VCOMH电压
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class SetVCOMH {
 public:
  static inline constexpr std::byte COMMAND_SET_VCOMH{0xDB};

 private:
  uint8_t m_vcomh_deselect_level;

 public:
  explicit SetVCOMH(uint8_t vcomh_deselect_level)
      : m_vcomh_deselect_level(vcomh_deselect_level) {}

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    auto data = std::array<std::byte, 2>(
        {COMMAND_SET_VCOMH, static_cast<std::byte>(m_vcomh_deselect_level)});
    ctx->renderer()->command(data);
  }
};

/**
 * @brief 设置显示起始行
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class SetDisplayStartLine {
 public:
  static inline constexpr std::byte COMMAND_SET_DISPLAY_START_LINE{0x40};

 private:
  uint8_t m_start_line;

 public:
  explicit SetDisplayStartLine(uint8_t start_line) : m_start_line(start_line) {}

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    auto data = std::array<std::byte, 1>(
        {COMMAND_SET_DISPLAY_START_LINE | std::byte{m_start_line}});
    ctx->renderer()->command(data);
  }
};

/**
 * @brief 设置扫描方向
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class SetSegmentRemap {
 public:
  static inline constexpr std::byte COMMAND_SET_SEGMENT_NORMAL{0xA0};
  static inline constexpr std::byte COMMAND_SET_SEGMENT_REMAP{0xA1};

 private:
  bool m_remap;

 public:
  explicit SetSegmentRemap(bool remap) : m_remap(remap) {}

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    auto data = std::array<std::byte, 1>(
        {m_remap ? COMMAND_SET_SEGMENT_REMAP : COMMAND_SET_SEGMENT_NORMAL});
    ctx->renderer()->command(data);
  }
};

// alias for SetSegmentRemap
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
using SetHorizontalFlip = SetSegmentRemap<Pl>;

/**
 * @brief 设置MUX比例
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class SetMultiplexRatio {
 public:
  static inline constexpr std::byte COMMAND_SET_MULTIPLEX_RATIO{0xA8};

 private:
  uint8_t m_ratio;

 public:
  explicit SetMultiplexRatio(uint8_t ratio) : m_ratio(ratio) {}

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    auto data = std::array<std::byte, 2>(
        {COMMAND_SET_MULTIPLEX_RATIO, static_cast<std::byte>(m_ratio)});
    ctx->renderer()->command(data);
  }
};

/**
 * @brief 设置COM输出扫描方向
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class SetComOutputScanDirection {
 public:
  static inline constexpr std::byte COMMAND_SET_COM_OUTPUT_SCAN_DIRECTION{0xC0};
  static inline constexpr std::byte COMMAND_SET_COM_OUTPUT_SCAN_DIRECTION_REMAP{
      0xC8};

 private:
  bool m_reverse;

 public:
  explicit SetComOutputScanDirection(bool reverse) : m_reverse(reverse) {}

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    auto data = std::array<std::byte, 1>(
        {m_reverse ? COMMAND_SET_COM_OUTPUT_SCAN_DIRECTION_REMAP
                   : COMMAND_SET_COM_OUTPUT_SCAN_DIRECTION});
    ctx->renderer()->command(data);
  }
};

template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
// alias for SetComOutputScanDirection
using SetVerticalFlip = SetComOutputScanDirection<Pl>;

/**
 * @brief 设置显示偏移
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class SetDisplayOffset {
 public:
  static inline constexpr std::byte COMMAND_SET_DISPLAY_OFFSET{0xD3};

 private:
  uint8_t m_offset;

 public:
  explicit SetDisplayOffset(uint8_t offset) : m_offset(offset) {}

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    auto data = std::array<std::byte, 2>(
        {COMMAND_SET_DISPLAY_OFFSET, static_cast<std::byte>(m_offset)});
    ctx->renderer()->command(data);
  }
};

/**
 * @brief 设置硬件配置
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class SetComPinsHardwareConfiguration {
 public:
  static inline constexpr std::byte COMMAND_SET_COM_PINS_HARDWARE_CONFIGURATION{
      0xDA};

 private:
  ComPinsConfig m_config;

 public:
  explicit SetComPinsHardwareConfiguration(ComPinsConfig config)
      : m_config(config) {}

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    auto data =
        std::array<std::byte, 2>({COMMAND_SET_COM_PINS_HARDWARE_CONFIGURATION,
                                  static_cast<std::byte>(m_config)});
    ctx->renderer()->command(data);
  }
};

/**
 * @brief 设置时钟分频和频率
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class SetClockRatioAndFrequency {
 public:
  static inline constexpr std::byte COMMAND_SET_CLOCK_RATIO_AND_FREQUENCY{0xD5};

 private:
  uint8_t m_ratio;
  uint8_t m_frequency;

 public:
  SetClockRatioAndFrequency(uint8_t ratio, uint8_t frequency)
      : m_ratio(ratio), m_frequency(frequency) {}

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    auto data = std::array<std::byte, 2>(
        {COMMAND_SET_CLOCK_RATIO_AND_FREQUENCY,
         static_cast<std::byte>((m_frequency << 4) | m_ratio)});
    ctx->renderer()->command(data);
  }
};

/**
 * @brief 设置充电泵
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class SetChargePump {
 public:
  static inline constexpr std::byte COMMAND_SET_CHARGE_PUMP{0x8D};
  static inline constexpr std::byte COMMAND_SET_CHARGE_PUMP_ENABLE{0x14};
  static inline constexpr std::byte COMMAND_SET_CHARGE_PUMP_DISABLE{0x10};

 private:
  bool m_enable;

 public:
  explicit SetChargePump(bool enable) : m_enable(enable) {}

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    auto data = std::array<std::byte, 2>(
        {COMMAND_SET_CHARGE_PUMP, m_enable ? COMMAND_SET_CHARGE_PUMP_ENABLE
                                           : COMMAND_SET_CHARGE_PUMP_DISABLE});
    ctx->renderer()->command(data);
  }
};

/**
 * @brief 初始化器
 */
template <typename Pl>
  requires SSDUI::Platform::IsPlatformDerivedFrom<SSD1306, Pl>
class Initializer {
 public:
  static inline constexpr uint8_t INIT_DELAY = 100;

  void operator()(SSDUI::Context::Context<Pl>* ctx) const {
    delay(INIT_DELAY);
    auto& config = ctx->config();

    SetDisplay<Pl>(false)(ctx);
    SetClockRatioAndFrequency<Pl>(config.clock_ratio,
                                  config.clock_frequency)(ctx);
    SetMultiplexRatio<Pl>(config.multiplex_ratio)(ctx);
    SetDisplayOffset<Pl>(config.display_offset)(ctx);
    SetAddressingMode<Pl>(config.addressing_mode)(ctx);
    SetDisplayStartLine<Pl>(config.start_line)(ctx);
    SetSegmentRemap<Pl>(config.horizontal_flip)(ctx);
    SetComOutputScanDirection<Pl>(config.vertical_flip)(ctx);
    SetComPinsHardwareConfiguration<Pl>(config.com_pins)(ctx);
    SetPrechargePeriod<Pl>(config.precharge_phase1,
                           config.precharge_phase2)(ctx);
    SetVCOMH<Pl>(config.vcomh_level)(ctx);
    SetEntireDisplay<Pl>(config.entire_display_on)(ctx);
    SetInvert<Pl>(config.inverse_display)(ctx);
    SetChargePump<Pl>(config.charge_pump_enable)(ctx);
    SetDisplay<Pl>(config.display_on)(ctx);
  }
};
}  // namespace SSD1306