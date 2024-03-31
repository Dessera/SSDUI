#pragma once

#include <cstddef>
#include <cstdint>

#include "ssdui/common/concepts.hh"

namespace SSDUI::Platform {

template <typename Co>
concept IsConfig = requires(Co config) {
  { config.width } -> std::same_as<std::int16_t>;
  { config.height } -> std::same_as<std::int16_t>;
  { config.fps } -> std::same_as<std::size_t>;
  { config.vertical_flip } -> std::same_as<bool>;
  { config.horizontal_flip } -> std::same_as<bool>;
};

}  // namespace SSDUI::Platform