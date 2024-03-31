#pragma once

#include <ssd1306_config.hh>
#include <ssd1306_renderer.hh>

enum class GlutEvent {
  KeyUp,
  KeyDown,
  KeyLeft,
  KeyRight,
  // KEY_DOWN,
  // KEY_LEFT,
  // KEY_RIGHT,
};

class GlutPlatform {
 public:
  using Renderer = SSD1306::Renderer;
  using Config = SSD1306::Config;
  using Event = GlutEvent;
};
