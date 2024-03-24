#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <thread>

#include "ssdui/common/geometry.hh"
#include "ssdui/context/buffer.hh"
#include "ssdui/context/config.hh"
#include "ssdui/render/concept.hh"

namespace ssdui::context {

template <typename Rnd>
  requires render::IsRenderer<Rnd>
class Context;

namespace _helper {

/**
 * @brief 组件类型的静态接口，由组件头文件拷贝（为了避免循环依赖）
 */
template <typename Cmp, typename Rnd>
concept IsComponent = requires(Cmp comp, context::Context<Rnd> *ctx) {
  { comp(ctx) };
};

/**
 * @brief 组件类型的动态接口
 */
template <typename Rnd>
  requires render::IsRenderer<Rnd>
class BaseComponent {
 public:
  virtual void operator()(context::Context<Rnd> *ctx) = 0;
};

/**
 * @brief 显示的根组件，仅做转发，为了缩减模板参数
 */
template <typename Cmp, typename Rnd>
  requires render::IsRenderer<Rnd> && IsComponent<Cmp, Rnd>
class RootComponent : public BaseComponent<Rnd> {
 private:
  Cmp m_cmp;

 public:
  explicit RootComponent(Cmp &&cmp) : m_cmp(std::move(cmp)) {}

  void operator()(context::Context<Rnd> *ctx) override { m_cmp(ctx); }
};

}  // namespace _helper

/**
 * @brief 显示上下文对象，用于管理显示的所有API
 */
template <typename Rnd>
  requires render::IsRenderer<Rnd>
class Context {
 public:
  using Self = Context;

 private:
  Config m_config;
  std::unique_ptr<Rnd> m_renderer;
  std::unique_ptr<_helper::BaseComponent<Rnd>> m_root{nullptr};
  CachedBuffer m_buffer;

 public:
  // // private constructor
  explicit Context(const Config &config, std::unique_ptr<Rnd> &&renderer)
      : m_config(config),
        m_renderer(std::move(renderer)),
        m_buffer(config.width, config.page) {}
  // Getters
  [[nodiscard]] const Config &config() const { return m_config; }
  [[nodiscard]] Rnd *renderer() const { return m_renderer.get(); }
  [[nodiscard]] CachedBuffer &buffer() { return m_buffer; }
  [[nodiscard]] _helper::BaseComponent<Rnd> *root() const {
    return m_root.get();
  }

  // template <typename Cmp>
  //   requires _helper::IsComponent<Cmp, Rnd>
  // Self &run(const Cmp &comp) {
  //   comp(this);
  // }

  template <typename Cmp>
    requires _helper::IsComponent<Cmp, Rnd>
  void mount(Cmp &&comp) {
    m_root =
        std::make_unique<_helper::RootComponent<Cmp, Rnd>>(std::move(comp));
  }
};
}  // namespace ssdui::context
