#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <thread>

#include "ssdui/context/buffer.hh"
#include "ssdui/context/config.hh"
#include "ssdui/render/concept.hh"

namespace ssdui::context {

template <typename Rnd>
  requires render::IsRenderer<Rnd>
class Context;

template <typename Rnd>
  requires render::IsRenderer<Rnd>
class ContextBuilder;

namespace _helper {

/**
 * @brief Copy of IsComponent concept, to avoid circular dependency
 */
template <typename Cmp, typename Rnd>
concept IsComponent = requires(Cmp comp) {
  { comp(std::declval<context::Context<Rnd> *>()) };
};

/**
 * @brief A dynamic interface for Component type
 */
template <typename Rnd>
  requires render::IsRenderer<Rnd>
class BaseComponent {
public:
  virtual void operator()(context::Context<Rnd> *ctx) = 0;
};

/**
 * @brief An explicit root component to avoid massive context template params
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

} // namespace _helper

/**
 * @brief Context UI object
 *
 * @tparam Rnd Renderer type
 */
template <typename Rnd>
  requires render::IsRenderer<Rnd>
class Context {
  friend class ContextBuilder<Rnd>;

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
      : m_config(config), m_renderer(std::move(renderer)),
        m_buffer(config.width, config.page) {}
  // Getters
  [[nodiscard]] const Config &config() const { return m_config; }
  [[nodiscard]] Rnd *renderer() const { return m_renderer.get(); }
  [[nodiscard]] CachedBuffer &buffer() { return m_buffer; }
  [[nodiscard]] _helper::BaseComponent<Rnd>*root() const {
    return m_root.get();
  }

  template <typename Cmp>
    requires _helper::IsComponent<Cmp, Rnd>
  Self &run(const Cmp &comp) {
    comp(this);
    return *this;
  }

  template <typename Cmp>
    requires _helper::IsComponent<Cmp, Rnd>
  Self &mount(std::unique_ptr<Cmp> &&comp) {
    m_root = std::move(comp);
    // directly call run() will cause error
    m_root->operator()(this);
    return *this;
  }
};

/**
 * @brief Builder for Context
 *
 * @tparam Rnd Renderer type
 */
// template <typename Rnd>
//   requires render::IsRenderer<Rnd>
// class ContextBuilder {
//  public:
//   using Self = ContextBuilder;

//  private:
//   std::optional<Config> m_config{std::nullopt};
//   std::shared_ptr<Rnd> m_renderer{nullptr};

//  public:
//   // Setters
//   Self& config(const Config& cfg) {
//     m_config = cfg;
//     return *this;
//   }

//   Self& renderer(std::unique_ptr<Rnd>&& renderer) {
//     m_renderer = std::move(renderer);
//     return *this;
//   }

//   // Build
//   std::unique_ptr<Context<Rnd>> build() {
//     if (!m_config.has_value() || m_renderer == nullptr) {
//       throw std::runtime_error("ContextBuilder: missing config or renderer");
//     }
//     return std::make_unique<Context<Rnd>>(m_config.value(),
//                                           std::move(m_renderer));
//   }
// };

template<typename Rnd>
class GlobalContextManager {
 private:
  std::unique_ptr<Context<Rnd>> m_ctx;
  std::thread m_update_handler;

  explicit GlobalContextManager(std::unique_ptr<Rnd>&& ctx): m_ctx(std::move(ctx)) {
    m_update_handler = std::thread([this]() {

    });
  }
  ~GlobalContextManager() { m_update_handler.join(); }
};

} // namespace ssdui::context
