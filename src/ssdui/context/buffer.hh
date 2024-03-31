#pragma once

#include <cstddef>
#include <memory>

#include "ssdui/common/span.hh"
namespace SSDUI::Context {

class Buffer {
 private:
  std::byte* prev_;
  std::byte* next_;

  std::size_t width_;
  std::size_t height_;

 public:
  Buffer(std::size_t width, std::size_t height);
  ~Buffer();

  Buffer(const Buffer& other);
  Buffer& operator=(const Buffer& other);
  Buffer(Buffer&& other) noexcept;
  Buffer& operator=(Buffer&& other) noexcept;

  [[nodiscard]] std::span<std::byte> prev() const {
    return {prev_, width_ * height_};
  }
  [[nodiscard]] std::span<std::byte> next() const {
    return {next_, width_ * height_};
  }

  [[nodiscard]] std::size_t width() const { return width_; }
  [[nodiscard]] std::size_t height() const { return height_; }

  void swap() noexcept { std::swap(prev_, next_); }
};

}  // namespace SSDUI::Context