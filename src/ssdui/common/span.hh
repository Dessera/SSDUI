#pragma once

#include <array>
#include <cstddef>
#include <initializer_list>
#include <vector>
namespace ssdui::common {

// implementation of dynamic std::span
// environment of esp-idf does not support std::span

template <typename T>
class Span {
 private:
  T *m_data;
  size_t m_size;

 public:
  Span(T *data, size_t size) : m_data(data), m_size(size) {}

  // enable more constructors

  // initializer list
  Span(std::initializer_list<T> list)
      : m_data(const_cast<T *>(list.begin())), m_size(list.size()) {}

  ~Span() = default;

  // array
  template <std::size_t N>
  explicit Span(const std::array<T, N> &array)
      : m_data(array.data()), m_size(N) {}

  explicit Span(const std::vector<T> &vector)
      : m_data(vector.data()), m_size(vector.size()) {}

  Span(const Span<T> &other) = default;
  Span(Span<T> &&other) = default;
  Span<T> &operator=(const Span<T> &other) = default;
  Span<T> &operator=(Span<T> &&other) = default;

  T *data() { return m_data; }
  const T *data() const { return m_data; }

  std::size_t size() const { return m_size; }

  T &operator[](std::size_t index) { return m_data[index]; }
  const T &operator[](std::size_t index) const { return m_data[index]; }

  T *begin() { return m_data; }
  const T *begin() const { return m_data; }

  T *end() { return m_data + m_size; }
  const T *end() const { return m_data + m_size; }

  Span<T> subspan(std::size_t offset, std::size_t count) {
    return Span<T>(m_data + offset, count);
  }
};

}  // namespace ssdui::common
