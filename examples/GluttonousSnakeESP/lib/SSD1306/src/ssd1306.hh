#pragma once

#include "ssd1306_config.hh"
#include "ssd1306_renderer.hh"
namespace SSD1306 {

class SSD1306 {
 public:
  using Renderer = ::SSD1306::Renderer;
  using Config = ::SSD1306::Config;

  enum class Event { None };

  struct Store {};
};

}  // namespace SSD1306
