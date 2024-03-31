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
  SSDUI::Geometry::Point<std::int32_t> position_{_generate_position()};

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
        GlutEvent::FoodEaten, "food_eaten",
        [this](auto) { position_ = _generate_position(); });
  }

  void operator()(SSDUI::Context::Context<GlutPlatform>* context) override {
    SSDUI::Components::Rectangle<GlutPlatform>{{position_, {WIDTH, WIDTH}}}(
        context);
  }

  [[nodiscard]] SSDUI::Geometry::Point<std::int32_t> get_position() const {
    return position_;
  }

  // void reset_position() { position_ = _generate_position(); }
};