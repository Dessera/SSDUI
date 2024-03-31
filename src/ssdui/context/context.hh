#pragma once

#include <memory>
#include <optional>

#include "ssdui/context/buffer.hh"
#include "ssdui/context/component.hh"
#include "ssdui/context/event.hh"
#include "ssdui/platform/concepts.hh"
namespace SSDUI::Context {

template <typename Pl>
class Builder;

template <typename Pl>
  requires Platform::IsPlatform<Pl>
class Context {
  friend class Builder<Pl>;

 public:
  using Platform = Pl;

  using Renderer = typename Platform::Renderer;
  using Config = typename Platform::Config;

 private:
  /**
   * @brief 渲染器，不分享所有权，但需要为组件提供渲染服务
   */
  std::unique_ptr<Renderer> renderer_;

  /**
   * @brief 配置，作为纯粹的数据结构，不提供任何服务
   */
  Config config_;

  /**
   * @brief 根组件，为保证唯一性，不分享所有权
   */
  std::unique_ptr<BaseComponent<Pl>> root_;

  /**
   * @brief Buffer，用于存储渲染结果
   * TODO: 未来可能会支持不同种类的Buffer
   */
  Buffer buffer_;

  /**
   * @brief 事件管理器，用于处理事件
   */
  EventManager<Pl> event_manager_{};

  Context(std::unique_ptr<Renderer> renderer, Config config,
          std::unique_ptr<BaseComponent<Pl>> root)
      : renderer_(std::move(renderer)),
        config_(std::move(config)),
        root_(std::move(root)),
        // TODO(dessera): 页大小应该由Buffer自己管理
        // TODO(dessera): Buffer应当是渲染器的一部分
        buffer_(config.width, config.height / 8) {}

 public:
  ~Context() = default;
  Context(const Context&) = delete;
  Context(Context&&) = delete;
  Context& operator=(const Context&) = delete;
  Context& operator=(Context&&) = delete;

  /**
   * @brief 获取渲染器
   *
   * @return Renderer*
   */
  Renderer* renderer() { return renderer_.get(); }

  /**
   * @brief 获取配置
   *
   * @return Config
   */
  Config& config() { return config_; }

  /**
   * @brief 获取根组件
   *
   * @return BaseComponent<Pl>*
   */
  BaseComponent<Pl>* root() { return root_.get(); }

  /**
   * @brief 获取Buffer
   *
   * @return Buffer*
   */
  Buffer& buffer() { return buffer_; }

  /**
   * @brief 获取事件管理器
   *
   * @return EventManager<Pl>&
   */
  EventManager<Pl>& event_manager() { return event_manager_; }

  // TODO(dessera): 应当有一个更好的方法来处理这个问题（Ticker的包含关系）
  template <typename Ti>
  static std::unique_ptr<Ti> to_ticker(std::unique_ptr<Context<Pl>> context) {
    return std::make_unique<Ti>(std::move(context));
  }
};

template <typename Pl>
class Builder {
 public:
  using Platform = Pl;

  using Renderer = typename Platform::Renderer;
  using Config = typename Platform::Config;

 private:
  std::unique_ptr<Renderer> renderer_;
  Config config_;
  std::unique_ptr<BaseComponent<Pl>> root_;

 public:
  Builder() = default;

  Builder& set_renderer(std::unique_ptr<Renderer> renderer) {
    renderer_ = std::move(renderer);
    return *this;
  }

  Builder& set_config(Config config) {
    config_ = std::move(config);
    return *this;
  }

  Builder& set_root(std::unique_ptr<BaseComponent<Pl>> root) {
    root_ = std::move(root);
    return *this;
  }

  // TODO(dessera): std::optional -> std::expected
  std::optional<std::unique_ptr<Context<Pl>>> build() {
    if (renderer_ == nullptr || root_ == nullptr) {
      return std::nullopt;
    }
    return std::make_unique<Context<Pl>>(std::move(renderer_),
                                         std::move(config_), std::move(root_));
  }
};
}  // namespace SSDUI::Context