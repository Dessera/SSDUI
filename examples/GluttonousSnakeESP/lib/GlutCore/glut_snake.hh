#pragma once

#include <condition_variable>
#include <cstdint>
#include <thread>
#include <vector>

#include "HardwareSerial.h"
#include "glut_food.hh"
#include "glut_platform.hh"
#include "ssdui/components/geometry.hh"
#include "ssdui/context/context.hh"
#include "ssdui/geometry/point.hh"

enum class GlutSnakeDirection { Up, Down, Left, Right };

class GlutSnake : public SSDUI::Context::BaseComponent<GlutPlatform> {
 public:
  static constexpr int32_t SNACK_SIZE = 4;

 private:
  std::vector<SSDUI::Geometry::Point<int32_t>> snake{{64, 32}};
  GlutSnakeDirection direction_{GlutSnakeDirection::Right};

  std::thread move_thread_;

  GlutFood food_{};

  void _move_handler(SSDUI::Context::Context<GlutPlatform>* context) {
    while (true) {
      auto head = snake.front();
      snake.pop_back();

      switch (direction_) {
        case GlutSnakeDirection::Up:
          head.y -= SNACK_SIZE;
          break;
        case GlutSnakeDirection::Down:
          head.y += SNACK_SIZE;
          break;
        case GlutSnakeDirection::Left:
          head.x -= SNACK_SIZE;
          break;
        case GlutSnakeDirection::Right:
          head.x += SNACK_SIZE;
          break;
      }

      snake.insert(snake.begin(), head);

      // if snake get food, trigger FoodEaten
      auto [food_x, food_y] = food_.get_position();
      if (head.x == food_x && head.y == food_y) {
        context->event_manager().trigger_event(GlutEvent::FoodEaten);
        snake.push_back(snake.back());
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
  }

 public:
  GlutSnake() = default;
  ~GlutSnake() {
    if (move_thread_.joinable()) {
      move_thread_.join();
    }
  }

  void on_mount(SSDUI::Context::Context<GlutPlatform>* context) override {
    context->event_manager().register_event(
        GlutEvent::KeyUp, "key_up",
        [this](auto* /*ctx*/) { this->direction_ = GlutSnakeDirection::Up; });
    context->event_manager().register_event(
        GlutEvent::KeyDown, "key_down",
        [this](auto* /*ctx*/) { this->direction_ = GlutSnakeDirection::Down; });
    context->event_manager().register_event(
        GlutEvent::KeyLeft, "key_left",
        [this](auto* /*ctx*/) { this->direction_ = GlutSnakeDirection::Left; });
    context->event_manager().register_event(
        GlutEvent::KeyRight, "key_right", [this](auto* /*ctx*/) {
          this->direction_ = GlutSnakeDirection::Right;
        });

    // move_thread_ = std::thread(&GlutSnake::_move_handler, this);
    move_thread_ = std::thread(&GlutSnake::_move_handler, this, context);

    food_.on_mount(context);
  }

  void operator()(SSDUI::Context::Context<GlutPlatform>* context) override {
    // SSDUI::Components::Rectangle<GlutPlatform>{{{0, 0}, {128, 64}}}(context);
    for (const auto& point : snake) {
      SSDUI::Components::Rectangle<GlutPlatform>{
          {{point.x, point.y}, {SNACK_SIZE, SNACK_SIZE}}}(context);
    }

    food_(context);
  }
};