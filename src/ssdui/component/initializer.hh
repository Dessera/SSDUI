#pragma once

/**
 * @file initializer.hh
 * @author Dessera (1533653159@qq.com)
 * @brief 初始化命令，会根据配置对象完整的设置显示器的所有参数
 * @version 0.1
 * @date 2024-03-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <memory>

#include "esp32-hal.h"
#include "ssdui/component/address.hh"
#include "ssdui/component/fundamental.hh"
#include "ssdui/component/hardware.hh"
#include "ssdui/context/config.hh"
#include "ssdui/context/core.hh"

namespace ssdui::command::initializer {

/**
 * @brief 默认初始化器，会根据配置对象完整的设置显示器的所有参数
 *
 * @tparam Rnd 渲染器类型
 * @todo 初始化后的首次清屏
 */
template <typename Rnd>
  requires render::IsRenderer<Rnd>
class DefaultInitializer {
 public:
  static inline constexpr uint8_t INIT_DELAY = 100;

  void operator()(context::Context<Rnd>* ctx) const {
    using namespace command;

    context::RuntimeConfig& cfg = ctx->config();

    delay(INIT_DELAY);

    fundamental::SetDisplay<Rnd>(false)(ctx);
    hardware::SetClockRatioAndFrequency<Rnd>(cfg.clock_ratio(),
                                             cfg.clock_frequency())(ctx);
    hardware::SetMultiplexRatio<Rnd>(cfg.multiplex_ratio())(ctx);
    hardware::SetDisplayOffset<Rnd>(cfg.display_offset())(ctx);
    address::SetAddressingMode<Rnd>(cfg.addressing_mode())(ctx);
    hardware::SetDisplayStartLine<Rnd>(cfg.start_line())(ctx);
    hardware::SetSegmentRemap<Rnd>(cfg.horizontal_flip())(ctx);
    hardware::SetComOutputScanDirection<Rnd>(cfg.vertical_flip())(ctx);
    hardware::SetComPinsHardwareConfiguration<Rnd>(cfg.com_pins())(ctx);
    hardware::SetPrechargePeriod<Rnd>(cfg.precharge_phase1(),
                                      cfg.precharge_phase2())(ctx);
    hardware::SetVCOMH<Rnd>(cfg.vcomh_level())(ctx);
    fundamental::SetEntireDisplay<Rnd>(cfg.entire_display_on())(ctx);
    fundamental::SetInvert<Rnd>(cfg.inverse_display())(ctx);
    hardware::SetChargePump<Rnd>(cfg.charge_pump_enable())(ctx);
    address::SetPageAddress<Rnd>(cfg.page_start(), cfg.page_end())(ctx);
    address::SetColumnAddress<Rnd>(cfg.column_start(), cfg.column_end())(ctx);
    fundamental::SetDisplay<Rnd>(cfg.display_on())(ctx);
  }
};

}  // namespace ssdui::command::initializer