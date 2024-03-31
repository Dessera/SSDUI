#pragma once

/**
 *  事件管理器
 *
 *  事件管理器负责管理事件的注册和触发
 */
#include <any>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <queue>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "ssdui/platform/concepts.hh"
namespace SSDUI::Context {

template <typename Pl>
  requires Platform::IsPlatform<Pl>
class Context;

template <typename Pl>
  requires Platform::IsPlatform<Pl>
struct EventPayload {
  typename Pl::Event type;
  std::any data;
};

template <typename Pl>
  requires Platform::IsPlatform<Pl>
class EventManager {
 public:
  using Platform = Pl;
  using Event = typename Platform::Event;

 private:
  std::unordered_map<
      typename Pl::Event,
      std::vector<std::function<void(Context<Pl>*, const std::any&)>>>
      listeners_{};

  std::queue<EventPayload<Pl>> event_queue_{};
  std::thread event_thread_;
  std::mutex lock_;
  std::condition_variable event_cv_;

  void _event_loop(Context<Pl>* ctx) {
    while (true) {
      std::unique_lock<std::mutex> lock(lock_);
      event_cv_.wait(lock, [this] { return !event_queue_.empty(); });

      auto event = event_queue_.front();
      event_queue_.pop();

      for (const auto& listener : listeners_[event.type]) {
        listener(ctx, event.data);
      }
    }
  }

 public:
  ~EventManager() {
    if (event_thread_.joinable()) {
      event_thread_.join();
    }
  }

  EventManager() = default;

  // 因为线程的存在，所以禁止拷贝和移动
  // 需要指针来传递EventManager的所有权

  EventManager(const EventManager&) = delete;
  EventManager(EventManager&&) = delete;
  EventManager& operator=(const EventManager&) = delete;
  EventManager& operator=(EventManager&&) = delete;

  /**
   * @brief 注册事件监听器
   *
   * @param event 事件
   * @param listener 监听器
   */
  void register_event(Event event, std::function<void(Context<Pl>*)> listener) {
    listeners_[event].push_back(
        [listener](Context<Pl>* ctx, const std::any&) { listener(ctx); });
  }

  void register_event(
      Event event,
      std::function<void(Context<Pl>*, const std::any&)> listener) {
    listeners_[event].push_back(listener);
  }

  /**
   * @brief 触发事件
   *
   * @param event 事件
   */
  void trigger_event(Event event) {
    std::lock_guard<std::mutex> lock(lock_);
    // event_queue_.push(event);
    // event_cv_.notify_one();
    event_queue_.push(EventPayload<Pl>{.type = event, .data = std::any{}});
    event_cv_.notify_one();
  }

  void trigger_event(Event event, const std::any& data) {
    std::lock_guard<std::mutex> lock(lock_);
    event_queue_.push(EventPayload<Pl>{.type = event, .data = data});
    event_cv_.notify_one();
  }

  /**
   * @brief 启动事件管理器
   */
  void enable(Context<Pl>* ctx) {
    event_thread_ = std::thread([this, ctx]() { _event_loop(ctx); });
  }
};

}  // namespace SSDUI::Context