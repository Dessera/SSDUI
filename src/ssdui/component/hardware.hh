#pragma once

/**
 * @file hardware.hh
 * @author Dessera (1533653159@qq.com)
 * @brief 硬件配置命令，在初始化时设置，后不可更改
 * @version 0.1
 * @date 2024-03-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <cstdint>

#include "ssdui/context/config.hh"
#include "ssdui/context/core.hh"
#include "ssdui/render/concept.hh"
#include "ssdui/render/default.hh"

namespace ssdui::command::hardware {

/**
 * @brief 设置预充电周期
 *
 * @tparam Rnd 渲染器类型
 */
template <typename Rnd>
  requires render::IsRenderer<Rnd>
class SetPrechargePeriod {
 public:
  static inline constexpr uint8_t COMMAND_SET_PRECHARGE_PERIOD = 0xD9;

 private:
  uint8_t m_period_phase1;
  uint8_t m_period_phase2;

 public:
  explicit SetPrechargePeriod(uint8_t period_phase1, uint8_t period_phase2)
      : m_period_phase1(period_phase1), m_period_phase2(period_phase2) {}

  void operator()(context::Context<Rnd>* ctx) const {
    ctx->renderer()->command({COMMAND_SET_PRECHARGE_PERIOD,
                              (m_period_phase2 << 4) | m_period_phase1});
  }
};

/**
 * @brief 设置VCOMH电压
 *
 * @tparam Rnd 渲染器类型
 */
template <typename Rnd>
  requires render::IsRenderer<Rnd>
class SetVCOMH {
 public:
  static inline constexpr uint8_t COMMAND_SET_VCOMH = 0xDB;

 private:
  uint8_t m_vcomh_deselect_level;

 public:
  explicit SetVCOMH(uint8_t vcomh_deselect_level)
      : m_vcomh_deselect_level(vcomh_deselect_level) {}

  void operator()(context::Context<Rnd>* ctx) const {
    ctx->renderer()->command({COMMAND_SET_VCOMH, m_vcomh_deselect_level});
  }
};

/**
 * @brief 设置显示起始行
 *
 * @tparam Rnd 渲染器类型
 */
template <typename Rnd>
  requires render::IsRenderer<Rnd>
class SetDisplayStartLine {
 public:
  static inline constexpr uint8_t COMMAND_SET_DISPLAY_START_LINE = 0x40;

 private:
  uint8_t m_start_line;

 public:
  explicit SetDisplayStartLine(uint8_t start_line) : m_start_line(start_line) {}

  void operator()(context::Context<Rnd>* ctx) const {
    ctx->renderer()->command({COMMAND_SET_DISPLAY_START_LINE | m_start_line});
  }
};

/**
 * @brief 设置SEG重映射
 *
 * @tparam Rnd 渲染器类型
 */
template <typename Rnd>
  requires render::IsRenderer<Rnd>
class SetSegmentRemap {
 public:
  static inline constexpr uint8_t COMMAND_SET_SEGMENT_NORMAL = 0xA0;
  static inline constexpr uint8_t COMMAND_SET_SEGMENT_REMAP = 0xA1;

 private:
  bool m_remap;

 public:
  explicit SetSegmentRemap(bool remap) : m_remap(remap) {}

  void operator()(context::Context<Rnd>* ctx) const {
    ctx->renderer()->command(
        {m_remap ? COMMAND_SET_SEGMENT_REMAP : COMMAND_SET_SEGMENT_NORMAL});
  }
};

// alias for SetSegmentRemap
template <typename Rnd>
  requires render::IsRenderer<Rnd>
using SetHorizontalFlip = SetSegmentRemap<Rnd>;

/**
 * @brief 设置MUX比例
 *
 * @tparam Rnd 渲染器类型
 * @note 此处使用原始值 N 而非 N-1，因此相比SSD1306的原命令，要增加1
 */
template <typename Rnd>
  requires render::IsRenderer<Rnd>
class SetMultiplexRatio {
 public:
  static inline constexpr uint8_t COMMAND_SET_MULTIPLEX_RATIO = 0xA8;

 private:
  uint8_t m_ratio;

 public:
  explicit SetMultiplexRatio(uint8_t ratio) : m_ratio(ratio) {}

  void operator()(context::Context<Rnd>* ctx) const {
    ctx->renderer()->command({COMMAND_SET_MULTIPLEX_RATIO, m_ratio - 1});
  }
};

/**
 * @brief 设置COM输出扫描方向
 *
 * @tparam Rnd 渲染器类型
 */
template <typename Rnd>
  requires render::IsRenderer<Rnd>
class SetComOutputScanDirection {
 public:
  static inline constexpr uint8_t COMMAND_SET_COM_OUTPUT_SCAN_DIRECTION = 0xC0;
  static inline constexpr uint8_t COMMAND_SET_COM_OUTPUT_SCAN_DIRECTION_REMAP =
      0xC8;

 private:
  bool m_reverse;

 public:
  explicit SetComOutputScanDirection(bool reverse) : m_reverse(reverse) {}

  void operator()(context::Context<Rnd>* ctx) const {
    ctx->renderer()->command({m_reverse
                                  ? COMMAND_SET_COM_OUTPUT_SCAN_DIRECTION_REMAP
                                  : COMMAND_SET_COM_OUTPUT_SCAN_DIRECTION});
  }
};

// alias for SetComOutputScanDirection
template <typename Rnd>
  requires render::IsRenderer<Rnd>
using SetVerticalFlip = SetComOutputScanDirection<Rnd>;

/**
 * @brief 设置显示偏移
 *
 * @tparam Rnd 渲染器类型
 */
template <typename Rnd>
  requires render::IsRenderer<Rnd>
class SetDisplayOffset {
 public:
  static inline constexpr uint8_t COMMAND_SET_DISPLAY_OFFSET = 0xD3;

 private:
  uint8_t m_offset;

 public:
  explicit SetDisplayOffset(uint8_t offset) : m_offset(offset) {}

  void operator()(context::Context<Rnd>* ctx) const {
    ctx->renderer()->command({COMMAND_SET_DISPLAY_OFFSET, m_offset});
  }
};

/**
 * @brief 设置COM硬件配置
 *
 * @tparam Rnd 渲染器类型
 */
template <typename Rnd>
  requires render::IsRenderer<Rnd>
class SetComPinsHardwareConfiguration {
 public:
  static inline constexpr uint8_t COMMAND_SET_COM_PINS_HARDWARE_CONFIGURATION =
      0xDA;

 private:
  context::ComPinsConfig m_config;

 public:
  explicit SetComPinsHardwareConfiguration(context::ComPinsConfig config)
      : m_config(config) {}

  void operator()(context::Context<Rnd>* ctx) const {
    ctx->renderer()->command({COMMAND_SET_COM_PINS_HARDWARE_CONFIGURATION,
                              static_cast<uint8_t>(m_config)});
  }
};

/**
 * @brief 设置时钟分频和频率
 *
 * @tparam Rnd 渲染器类型
 */
template <typename Rnd>
  requires render::IsRenderer<Rnd>
class SetClockRatioAndFrequency {
 public:
  static inline constexpr uint8_t COMMAND_SET_CLOCK_RATIO_AND_FREQUENCY = 0xD5;

 private:
  uint8_t m_ratio;
  uint8_t m_frequency;

 public:
  SetClockRatioAndFrequency(uint8_t ratio, uint8_t frequency)
      : m_ratio(ratio), m_frequency(frequency) {}

  void operator()(context::Context<Rnd>* ctx) const {
    ctx->renderer()->command(
        {COMMAND_SET_CLOCK_RATIO_AND_FREQUENCY, (m_frequency << 4) | m_ratio});
  }
};

/**
 * @brief 设置充电泵
 *
 * @tparam Rnd 渲染器类型
 */
template <typename Rnd>
  requires render::IsRenderer<Rnd>
class SetChargePump {
 public:
  static inline constexpr uint8_t COMMAND_SET_CHARGE_PUMP = 0x8D;
  static inline constexpr uint8_t COMMAND_SET_CHARGE_PUMP_ENABLE = 0x14;
  static inline constexpr uint8_t COMMAND_SET_CHARGE_PUMP_DISABLE = 0x10;

 private:
  bool m_enable;

 public:
  explicit SetChargePump(bool enable) : m_enable(enable) {}

  void operator()(context::Context<Rnd>* ctx) const {
    ctx->renderer()->command(
        {COMMAND_SET_CHARGE_PUMP, m_enable ? COMMAND_SET_CHARGE_PUMP_ENABLE
                                           : COMMAND_SET_CHARGE_PUMP_DISABLE});
  }
};

}  // namespace ssdui::command::hardware
