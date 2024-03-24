#pragma once

#include <cstddef>
#include <cstdint>

#include "ssdui/common/concepts_utils.hh"
#include "ssdui/common/span.hh"

namespace ssdui::render {

/**
 * @brief Renderer concept
 * @tparam T A type to be checked
 */
template <typename T>
concept IsRenderer = requires(T rend, common::Span<uint8_t> data) {
  { rend.command(data) } -> common::SameAs<std::size_t>;
  { rend.data(data) } -> common::SameAs<std::size_t>;
};

}  // namespace ssdui::render
