#pragma once

#include <OneButton.h>

#include <thread>

#include "glut_platform.hh"
#include "ssdui/context/component.hh"
#include "ssdui/context/context.hh"
class GlutEventScanner : SSDUI::Context::BaseComponent<GlutPlatform> {
 private:
  OneButton key_up_{8, true, true};
  OneButton key_down_{12, true, true};
  OneButton key_left_{3, true, true};
  OneButton key_right_{7, true, true};

  std::thread button_thread_;

 public:
  GlutEventScanner() {
    gpio_set_direction(GPIO_NUM_12, GPIO_MODE_INPUT_OUTPUT);
    gpio_set_level(GPIO_NUM_12, HIGH);
  }

  ~GlutEventScanner() {
    if (button_thread_.joinable()) {
      button_thread_.join();
    }
  }

  void on_mount(SSDUI::Context::Context<GlutPlatform>* context) override {
    auto& emgr = context->event_manager();
    key_up_.attachClick(
        [](void* emgr) {
          auto* em =
              static_cast<SSDUI::Context::EventManager<GlutPlatform>*>(emgr);
          em->trigger_event(GlutEvent::KeyUp);
        },
        (void*)(&emgr));
    key_down_.attachClick(
        [](void* emgr) {
          auto* em =
              static_cast<SSDUI::Context::EventManager<GlutPlatform>*>(emgr);
          em->trigger_event(GlutEvent::KeyDown);
        },
        (void*)(&emgr));
    key_left_.attachClick(
        [](void* emgr) {
          auto* em =
              static_cast<SSDUI::Context::EventManager<GlutPlatform>*>(emgr);
          em->trigger_event(GlutEvent::KeyLeft);
        },
        (void*)(&emgr));
    key_right_.attachClick(
        [](void* emgr) {
          auto* em =
              static_cast<SSDUI::Context::EventManager<GlutPlatform>*>(emgr);
          em->trigger_event(GlutEvent::KeyRight);
        },
        (void*)(&emgr));

    button_thread_ = std::thread([this]() {
      while (true) {
        key_up_.tick();
        key_down_.tick();
        key_left_.tick();
        key_right_.tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
    });
  }

  void operator()(SSDUI::Context::Context<GlutPlatform>* context) {}
};
