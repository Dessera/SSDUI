#include <Arduino.h>

#include <memory>
#include <ssd1306.hh>

#include "Wire.h"
#include "ssd1306_command.hh"
#include "ssd1306_config.hh"
#include "ssd1306_renderer.hh"
#include "ssd1306_ticker.hh"
#include "ssdui/context/component.hh"
#include "ssdui/context/context.hh"

std::unique_ptr<SSD1306::Ticker<SSD1306::SSD1306>> ticker{nullptr};

class TestComponent : public SSDUI::Context::BaseComponent<SSD1306::SSD1306> {
 public:
  void operator()(SSDUI::Context::Context<SSD1306::SSD1306>* context) override {
    //
    context->buffer().set(0, 0, std::byte{0xFF});
    context->buffer().set(1, 0, std::byte{0xFF});
    context->buffer().set(2, 0, std::byte{0xFF});
    context->buffer().set(3, 0, std::byte{0xFF});
    context->buffer().set(4, 0, std::byte{0xFF});
  }
};

void setup() {
  namespace Context = SSDUI::Context;

  Serial.begin(115200);

  auto opt = Context::Builder<SSD1306::SSD1306>()
                 .set_config(SSD1306::Config{})
                 .set_renderer(std::make_unique<SSD1306::Renderer>(&Wire, 0x3C))
                 .set_root(std::make_unique<TestComponent>())
                 .build();

  if (!opt.has_value()) {
    Serial.println("Failed to create context");
    return;
  }

  auto context = std::move(opt.value());

  SSD1306::Initializer()(context.get());

  ticker = Context::Context<SSD1306::SSD1306>::to_ticker<
      SSD1306::Ticker<SSD1306::SSD1306>>(std::move(context));
}

void loop() {}