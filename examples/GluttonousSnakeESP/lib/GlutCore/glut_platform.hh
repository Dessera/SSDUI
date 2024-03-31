#pragma once

#include <ssd1306_config.hh>
#include <ssd1306_renderer.hh>

enum class GlutEvent {

  // 控制事件
  KeyUp,
  KeyDown,
  KeyLeft,
  KeyRight,

  // 游戏全局
  GameStart,
  GameOver,

  // 内部触发
  FoodEaten,
};

enum class GlutState {
  Ready,
  Running,
  Failed,
};

class GlutPlatform {
 public:
  using Renderer = SSD1306::Renderer;
  using Config = SSD1306::Config;
  using Event = GlutEvent;
};
