#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "ssdui/common/concepts_utils.hh"

#define CHECK_RENDERER(T) \
  static_assert(ssdui::render::IsRenderer<T>, "T is not a renderer");

namespace ssdui::render {

/**
 * @brief Renderer concept
 * @tparam T A type to be checked
 */
template <typename T>
concept IsRenderer = requires(T rend) {
  { rend.command(std::vector<uint8_t>{}) } -> common::SameAs<std::size_t>;
  { rend.data(std::vector<uint8_t>{}) } -> common::SameAs<std::size_t>;
};

}  // namespace ssdui::render
