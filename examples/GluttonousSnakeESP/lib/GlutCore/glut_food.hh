#pragma once

#include <cstdint>
#include <random>
#include <ssdui/geometry/point.hh>

#include "glut_platform.hh"
#include "ssdui/components/geometry.hh"

class GlutFood : public SSDUI::Context::BaseComponent<GlutPlatform> {
 public:
  static constexpr std::int32_t WIDTH = 4;

 private:
  std::random_device rd_{};
  std::mt19937 gen_{rd_()};
  // SSDUI::Geometry::Point<std::int32_t> position_{_generate_position()};

  SSDUI::Geometry::Point<std::int32_t> _generate_position() {
    return {static_cast<int>(gen_() % 32) * 4,
            static_cast<int>(gen_() % 16) * 4};
  }

 public:
  GlutFood() = default;
  virtual ~GlutFood() = default;

  GlutFood(const GlutFood&) = delete;
  GlutFood& operator=(const GlutFood&) = delete;
  GlutFood(GlutFood&&) = delete;
  GlutFood& operator=(GlutFood&&) = delete;

  void on_mount(SSDUI::Context::Context<GlutPlatform>* context) override {
    context->event_manager().register_event(
        GlutEvent::FoodEaten, [this](auto* ctx) {
          ctx->store().food = _generate_position();
          ctx->store().score += 1;
        });
    context->event_manager().register_event(
        GlutEvent::GameStart,
        [this](auto* ctx) { ctx->store().food = _generate_position(); });
  }

  void operator()(SSDUI::Context::Context<GlutPlatform>* context) override {
    if (context->store().state == GlutState::Running) {
      SSDUI::Components::Rectangle<GlutPlatform>{
          {context->store().food, {WIDTH, WIDTH}}}(context);
    }
  }
};