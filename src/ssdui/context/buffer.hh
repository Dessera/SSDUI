#pragma once

#include <cstddef>

#include "ssdui/common/span.hh"
namespace SSDUI::Context {

class Buffer {
 private:
  std::span<std::byte> prev_;
  std::span<std::byte> next_;

  std::size_t width_;
  std::size_t height_;

 public:
  Buffer(std::size_t width, std::size_t height)
      : prev_(new std::byte[width * height], width * height),
        next_(new std::byte[width * height], width * height),
        width_(width),
        height_(height) {}

  ~Buffer() {
    delete[] prev_.data();
    delete[] next_.data();
  }

  std::span<std::byte> prev() { return prev_; }
  std::span<std::byte> next() { return next_; }

  [[nodiscard]] std::size_t width() const { return width_; }
  [[nodiscard]] std::size_t height() const { return height_; }

  void swap() noexcept { std::swap(prev_, next_); }
};

}  // namespace SSDUI::Context