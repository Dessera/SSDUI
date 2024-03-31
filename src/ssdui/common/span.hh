#pragma once

// 因为当前编译环境不存在 std::span, 用该文件显式扩充 std::span 的定义
// TODO(dessera): 后续会使用 GCC >= 12 以引入 std::span

#include <type_traits>
namespace std {

template <typename T>
class span {
 public:
  using element_type = T;
  using value_type = std::remove_cv_t<T>;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using iterator = T*;
  using const_iterator = const T*;

  constexpr span() noexcept : data_(nullptr), size_(0) {}
  constexpr span(std::nullptr_t) noexcept : span() {}
  constexpr span(T* data, size_type size) noexcept : data_(data), size_(size) {}
  template <size_type N>
  constexpr span(T (&data)[N]) noexcept : data_(data), size_(N) {}
  template <typename Container>
  constexpr span(Container& container) noexcept
      : data_(container.data()), size_(container.size()) {}

  constexpr T* data() const noexcept { return data_; }
  [[nodiscard]] constexpr size_type size() const noexcept { return size_; }
  [[nodiscard]] constexpr bool empty() const noexcept { return size_ == 0; }

  constexpr T& operator[](size_type index) const noexcept {
    return data_[index];
  }
  constexpr T& front() const noexcept { return data_[0]; }
  constexpr T& back() const noexcept { return data_[size_ - 1]; }

  constexpr T* begin() const noexcept { return data_; }
  constexpr T* end() const noexcept { return data_ + size_; }
  constexpr T* cbegin() const noexcept { return data_; }
  constexpr T* cend() const noexcept { return data_ + size_; }

  constexpr span<T> subspan(size_type offset, size_type count) const noexcept {
    return {data_ + offset, count};
  }

 private:
  T* data_;
  size_type size_;
};

}  // namespace std