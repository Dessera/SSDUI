#pragma once

#include <ssd1306.hh>
#include <ssdui/context/component.hh>

#include "glut_event.hh"
#include "glut_platform.hh"
#include "glut_snake.hh"
#include "ssdui/context/context.hh"

class GlutRoot : public SSDUI::Context::BaseComponent<GlutPlatform> {
 private:
  GlutEventScanner event_scanner_{};
  GlutSnake snake_{};

 public:
  GlutRoot() = default;
  virtual ~GlutRoot() = default;

  GlutRoot(const GlutRoot&) = delete;
  GlutRoot& operator=(const GlutRoot&) = delete;
  GlutRoot(GlutRoot&&) = delete;
  GlutRoot& operator=(GlutRoot&&) = delete;

  void on_mount(SSDUI::Context::Context<GlutPlatform>* context) override {
    event_scanner_.on_mount(context);
    snake_.on_mount(context);
  }

  void operator()(SSDUI::Context::Context<GlutPlatform>* context) override {
    snake_(context);
  }
};
