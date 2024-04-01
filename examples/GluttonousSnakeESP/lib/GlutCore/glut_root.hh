#pragma once

#include <ssd1306.hh>
#include <ssdui/context/component.hh>

#include "glut_event.hh"
#include "glut_food.hh"
#include "glut_platform.hh"
#include "glut_snake.hh"
#include "glut_store.hh"
#include "glut_string.hh"
#include "ssdui/context/context.hh"

class GlutRoot : public SSDUI::Context::BaseComponent<GlutPlatform> {
 private:
  GlutEventScanner event_scanner_{};
  GlutSnake snake_{};
  GlutFood food_{};

 public:
  GlutRoot() = default;
  virtual ~GlutRoot() = default;

  GlutRoot(const GlutRoot&) = delete;
  GlutRoot& operator=(const GlutRoot&) = delete;
  GlutRoot(GlutRoot&&) = delete;
  GlutRoot& operator=(GlutRoot&&) = delete;

  void on_mount(SSDUI::Context::Context<GlutPlatform>* context) override {
    // 触发游戏结束事件，同步全局状态
    context->event_manager().register_event(
        GlutEvent::GameOver,
        [this](auto* ctx) { ctx->store().state = GlutState::Failed; });

    // 触发游戏开始事件，同步全局状态
    context->event_manager().register_event(
        GlutEvent::GameStart,
        [this](auto* ctx) { ctx->store().state = GlutState::Running; });

    // 在game over或预备状态下，按任意键重新开始游戏
    context->event_manager().register_event(
        GlutEvent::KeyUp, [this](auto* ctx) {
          if (ctx->store().state == GlutState::Failed ||
              ctx->store().state == GlutState::Ready) {
            ctx->event_manager().trigger_event(GlutEvent::GameStart);
          }
        });
    context->event_manager().register_event(
        GlutEvent::KeyDown, [this](auto* ctx) {
          if (ctx->store().state == GlutState::Failed ||
              ctx->store().state == GlutState::Ready) {
            ctx->event_manager().trigger_event(GlutEvent::GameStart);
          }
        });
    context->event_manager().register_event(
        GlutEvent::KeyLeft, [this](auto* ctx) {
          if (ctx->store().state == GlutState::Failed ||
              ctx->store().state == GlutState::Ready) {
            ctx->event_manager().trigger_event(GlutEvent::GameStart);
          }
        });
    context->event_manager().register_event(
        GlutEvent::KeyRight, [this](auto* ctx) {
          if (ctx->store().state == GlutState::Failed ||
              ctx->store().state == GlutState::Ready) {
            ctx->event_manager().trigger_event(GlutEvent::GameStart);
          }
        });

    event_scanner_.on_mount(context);
    snake_.on_mount(context);
    food_.on_mount(context);
  }

  void operator()(SSDUI::Context::Context<GlutPlatform>* context) override {
    if (context->store().state == GlutState::Ready) {
      GlutString{"GluttonousSnake", {4, 12}}(context);
      GlutString{"Press any key", {12, 32}}(context);
    } else if (context->store().state == GlutState::Failed) {
      GlutString{"Game Over", {16, 12}}(context);
      GlutString{"Press any key", {12, 32}}(context);
    }
    snake_(context);
    food_(context);
  }
};
