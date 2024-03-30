#pragma once

#include <memory>

#include "ssdui/context/component.hh"
#include "ssdui/platform/concepts.hh"
namespace SSDUI::Context {

template <typename Pl>
  requires Platform::IsPlatform<Pl>
class Context {
 public:
  using Platform = Pl;

  using Renderer = typename Platform::Renderer;
  using Config = typename Platform::Config;

 private:
  /**
   * @brief 渲染器，不分享所有权，但需要为组件提供渲染服务
   */
  std::unique_ptr<Renderer> renderer_{};

  /**
   * @brief 配置，作为纯粹的数据结构，不提供任何服务
   */
  Config config_;

  /**
   * @brief 根组件，为保证唯一性，不分享所有权
   */
  std::unique_ptr<BaseComponent<Pl>> root_{};

 public:
};

}  // namespace SSDUI::Context