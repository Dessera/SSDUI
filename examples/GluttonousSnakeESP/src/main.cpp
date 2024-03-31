#include <Arduino.h>
#include <Wire.h>

#include <array>
#include <glut_root.hh>
#include <memory>
#include <ssd1306.hh>
#include <ssd1306_command.hh>
#include <ssd1306_config.hh>
#include <ssd1306_renderer.hh>
#include <ssd1306_ticker.hh>
#include <ssdui/context/component.hh>
#include <ssdui/context/context.hh>

#include "glut_platform.hh"
#include "ssdui/components/geometry.hh"

std::unique_ptr<SSD1306::Ticker<GlutPlatform>> ticker{nullptr};

void setup() {
  namespace Context = SSDUI::Context;

  Serial.begin(115200);

  auto opt = Context::Builder<GlutPlatform>()
                 .set_config(SSD1306::Config{})
                 .set_renderer(std::make_unique<SSD1306::Renderer>(&Wire, 0x3C))
                 .set_root(std::make_unique<GlutRoot>())
                 .build();

  if (!opt.has_value()) {
    Serial.println("Failed to create context");
    return;
  }

  auto context = std::move(opt.value());

  context->enable_event_manager();

  SSD1306::Initializer<GlutPlatform>()(context.get());

  ticker =
      Context::Context<GlutPlatform>::to_ticker<SSD1306::Ticker<GlutPlatform>>(
          std::move(context));
}

void loop() { delay(100); }