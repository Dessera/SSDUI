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

  void _move_handler(SSDUI::Context::Context<GlutPlatform>* context) {
    while (true) {
      if (context->store().state != GlutState::Running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        continue;
      }

      auto head = snake.front();
      snake.pop_back();

      switch (direction_) {
        case GlutSnakeDirection::Up:
          head.y -= SNACK_SIZE;
          // head.y = head.y < 0 ? 60 : head.y;
          break;
        case GlutSnakeDirection::Down:
          head.y += SNACK_SIZE;
          // head.y = head.y > 64 ? 0 : head.y;
          break;
        case GlutSnakeDirection::Left:
          head.x -= SNACK_SIZE;
          // head.x = head.x < 0 ? 124 : head.x;
          break;
        case GlutSnakeDirection::Right:
          head.x += SNACK_SIZE;
          // head.x = head.x > 128 ? 0 : head.x;
          break;
      }

      // overflow check
      head.x = head.x < 0 ? 124 : head.x;
      head.x = head.x > 124 ? 0 : head.x;
      head.y = head.y < 0 ? 60 : head.y;
      head.y = head.y > 60 ? 0 : head.y;

      snake.insert(snake.begin(), head);

      // if snake hit itself, trigger GameOver
      for (size_t i = 1; i < snake.size(); ++i) {
        if (head.x == snake[i].x && head.y == snake[i].y) {
          context->event_manager().trigger_event(GlutEvent::GameOver);
          break;
        }
      }

      // if snake get food, trigger FoodEaten
      auto [food_x, food_y] = context->store().food;
      if (head.x == food_x && head.y == food_y) {
        context->event_manager().trigger_event(GlutEvent::FoodEaten);
        // snake.push_back(snake.back());
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }

  void _reset() {
    snake.clear();
    snake.emplace_back(64, 32);
    direction_ = GlutSnakeDirection::Right;
  }

 public:
  GlutSnake() = default;
  ~GlutSnake() {
    if (move_thread_.joinable()) {
      move_thread_.join();
    }
  }

  void on_mount(SSDUI::Context::Context<GlutPlatform>* context) override {
    context->event_manager().register_event(GlutEvent::KeyUp, [this](auto) {
      direction_ = direction_ == GlutSnakeDirection::Down
                       ? GlutSnakeDirection::Down
                       : GlutSnakeDirection::Up;
    });
    context->event_manager().register_event(GlutEvent::KeyDown, [this](auto) {
      direction_ = direction_ == GlutSnakeDirection::Up
                       ? GlutSnakeDirection::Up
                       : GlutSnakeDirection::Down;
    });
    context->event_manager().register_event(GlutEvent::KeyLeft, [this](auto) {
      direction_ = direction_ == GlutSnakeDirection::Right
                       ? GlutSnakeDirection::Right
                       : GlutSnakeDirection::Left;
    });
    context->event_manager().register_event(GlutEvent::KeyRight, [this](auto) {
      direction_ = direction_ == GlutSnakeDirection::Left
                       ? GlutSnakeDirection::Left
                       : GlutSnakeDirection::Right;
    });
    context->event_manager().register_event(
        GlutEvent::FoodEaten, [this](auto) { snake.push_back(snake.back()); });
    context->event_manager().register_event(GlutEvent::GameStart,
                                            [this](auto) { _reset(); });

    move_thread_ = std::thread(&GlutSnake::_move_handler, this, context);
  }

  void operator()(SSDUI::Context::Context<GlutPlatform>* context) override {
    if (context->store().state == GlutState::Running) {
      for (const auto& point : snake) {
        SSDUI::Components::Rectangle<GlutPlatform>{
            {{point.x, point.y}, {SNACK_SIZE, SNACK_SIZE}}}(context);
      }
    }
  }
};