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

    const auto& cfg = ctx->config();

    delay(INIT_DELAY);

    ctx->run(fundamental::SetDisplay<Rnd>(false))
        .run(hardware::SetClockRatioAndFrequency<Rnd>(cfg.clock_ratio,
                                                      cfg.clock_frequency))
        .run(hardware::SetMultiplexRatio<Rnd>(cfg.multiplex_ratio))
        .run(hardware::SetDisplayOffset<Rnd>(cfg.display_offset))
        .run(address::SetAddressingMode<Rnd>(cfg.addressing_mode))
        .run(hardware::SetDisplayStartLine<Rnd>(cfg.start_line))
        .run(hardware::SetSegmentRemap<Rnd>(cfg.horizontal_flip))
        .run(hardware::SetComOutputScanDirection<Rnd>(cfg.vertical_flip))
        .run(hardware::SetComPinsHardwareConfiguration<Rnd>(cfg.com_pins))
        .run(hardware::SetPrechargePeriod<Rnd>(cfg.precharge_phase1,
                                               cfg.precharge_phase2))
        .run(hardware::SetVCOMH<Rnd>(cfg.vcomh_level))
        .run(fundamental::SetEntireDisplay<Rnd>(cfg.entire_display_on))
        .run(fundamental::SetInvert<Rnd>(cfg.inverse_display))
        .run(hardware::SetChargePump<Rnd>(cfg.charge_pump_enable))
        .run(address::SetPageAddress<Rnd>(cfg.page_start, cfg.page_end))
        .run(address::SetColumnAddress<Rnd>(cfg.column_start, cfg.column_end))
        .run(fundamental::SetDisplay<Rnd>(cfg.display_on));
  }
};

}  // namespace ssdui::command::initializer