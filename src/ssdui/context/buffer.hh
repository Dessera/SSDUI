#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

#include "HardwareSerial.h"
#include "ssdui/common/span.hh"
namespace SSDUI::Context {

class Buffer {
 private:
  std::uint8_t* prev_;
  std::uint8_t* next_;

  std::int16_t width_;
  std::int16_t height_;

 public:
  Buffer(std::int16_t width, std::int16_t height);
  ~Buffer();

  Buffer(const Buffer& other);
  Buffer& operator=(const Buffer& other);
  Buffer(Buffer&& other) noexcept;
  Buffer& operator=(Buffer&& other) noexcept;

  [[nodiscard]] std::span<std::uint8_t> prev() const {
    return {prev_, static_cast<std::size_t>(width_ * height_)};
  }
  [[nodiscard]] std::span<std::uint8_t> next() const {
    return {next_, static_cast<std::size_t>(width_ * height_)};
  }

  [[nodiscard]] std::int16_t width() const { return width_; }
  [[nodiscard]] std::int16_t height() const { return height_; }

  void swap() noexcept {
    std::swap(prev_, next_);
    // auto* temp = prev_;
    // prev_ = next_;
    // next_ = temp;
  }
  void clear() noexcept { std::fill(next_, next_ + width_ * height_, 0U); }

  void set(std::int16_t x, std::int16_t y, std::uint8_t value) {
    next_[x + y * width_] = value;
  }

  void mixin(std::int16_t x, std::int16_t y, std::uint8_t value) {
    next_[x + y * width_] |= value;
  }
};

}  // namespace SSDUI::Context