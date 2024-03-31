#include "ssdui/context/buffer.hh"

namespace SSDUI::Context {

Buffer::Buffer(std::size_t width, std::size_t height)
    : prev_(new std::byte[width * height]),
      next_(new std::byte[width * height]),
      width_(width),
      height_(height) {
  std::fill(prev_, prev_ + width * height, std::byte{0});
  std::fill(next_, next_ + width * height, std::byte{0xFF});
}

Buffer::~Buffer() {
  delete[] prev_;
  delete[] next_;
}

Buffer::Buffer(const Buffer& other)
    : prev_(new std::byte[other.width_ * other.height_]),
      next_(new std::byte[other.width_ * other.height_]),
      width_(other.width_),
      height_(other.height_) {
  std::copy(other.prev_, other.prev_ + width_ * height_, prev_);
  std::copy(other.next_, other.next_ + width_ * height_, next_);
}

Buffer::Buffer(Buffer&& other) noexcept
    : prev_(other.prev_),
      next_(other.next_),
      width_(other.width_),
      height_(other.height_) {
  other.prev_ = nullptr;
  other.next_ = nullptr;
  other.width_ = 0;
  other.height_ = 0;
}

Buffer& Buffer::operator=(const Buffer& other) {
  if (this == &other) {
    return *this;
  }

  delete[] prev_;
  delete[] next_;

  prev_ = new std::byte[other.width_ * other.height_];
  next_ = new std::byte[other.width_ * other.height_];
  width_ = other.width_;
  height_ = other.height_;

  std::copy(other.prev_, other.prev_ + width_ * height_, prev_);
  std::copy(other.next_, other.next_ + width_ * height_, next_);

  return *this;
}

Buffer& Buffer::operator=(Buffer&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  delete[] prev_;
  delete[] next_;

  prev_ = other.prev_;
  next_ = other.next_;
  width_ = other.width_;
  height_ = other.height_;

  other.prev_ = nullptr;
  other.next_ = nullptr;
  other.width_ = 0;
  other.height_ = 0;

  return *this;
}

}  // namespace SSDUI::Context