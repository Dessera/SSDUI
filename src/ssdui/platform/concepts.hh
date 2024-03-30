#pragma once

/**
 *  SSDUI Platform 概念
 *
 *  平台是对于 SSDUI 的底层抽象，从上下文中隔离平台相关的代码。
 *  平台应当包含以下内容：
 *  - 渲染器 (Renderer): 负责渲染实现
 *  - 配置信息 (Config): 包含平台相关的配置信息
 */

#include <cstddef>
#include <cstdint>
#include <ssdui/common/concepts.hh>
#include <ssdui/common/span.hh>

namespace SSDUI::Platform {

template <typename Re>
concept IsRenderer = requires(Re rend, std::span<uint8_t> data) {
  { rend.command(data) } -> std::same_as<std::size_t>;
  { rend.data(data) } -> std::same_as<std::size_t>;
};

template <typename Co>
concept IsConfig = requires(Co config) {
  { config.width } -> std::same_as<std::size_t&>;
  { config.height } -> std::same_as<std::size_t&>;
  { config.fps } -> std::same_as<std::size_t&>;
  { config.vertical_flip } -> std::same_as<bool&>;
  { config.horizontal_flip } -> std::same_as<bool&>;
};

template <typename Pl>
concept IsPlatform = requires {
  /**
   * @brief 平台的渲染器类型
   */
  requires IsRenderer<typename Pl::Renderer>;

  /**
   * @brief 平台的配置信息类型
   */
  requires IsConfig<typename Pl::Config>;
};

}  // namespace SSDUI::Platform