#pragma once

#include <memory>

#include "ssdui/platform/concepts.hh"

namespace SSDUI::Context {

template <typename Pl>
  requires Platform::IsPlatform<Pl>
class Context;

template <typename Pl, typename Cm>
concept IsComponent = requires(Cm comp, Context<Pl> *ctx) {
  { comp(ctx) };
  { comp.on_mount(ctx) };
};

template <typename Pl>
class BaseComponent {
 public:
  virtual void operator()(Context<Pl> *ctx) = 0;
  virtual void on_mount(Context<Pl> *ctx) {}
};

template <typename Pl, IsComponent<Pl> Cm>
class Component : public BaseComponent<Pl> {
  std::unique_ptr<Cm> comp_;

 public:
  explicit Component(std::unique_ptr<Cm> comp) : comp_(std::move(comp)) {}

  void operator()(Context<Pl> *ctx) override { (*comp_)(ctx); }
  void on_mount(Context<Pl> *ctx) override { comp_->on_mount(ctx); }
};

}  // namespace SSDUI::Context