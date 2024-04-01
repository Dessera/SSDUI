#pragma once

#include <cstddef>
#include <cstdint>

#include "ssdui/common/concepts.hh"

namespace SSDUI::Platform {

template <typename Co>
concept IsConfig = requires(Co config) {
  {
    std::remove_reference_t<decltype(config.width)>{}
  } -> std::same_as<std::int16_t>;

  {
    std::remove_reference_t<decltype(config.height)>{}
  } -> std::same_as<std::int16_t>;

  {
    std::remove_reference_t<decltype(config.fps)>{}
  } -> std::same_as<std::int16_t>;

  {
    std::remove_reference_t<decltype(config.vertical_flip)>{}
  } -> std::same_as<bool>;

  {
    std::remove_reference_t<decltype(config.horizontal_flip)>{}
  } -> std::same_as<bool>;
};

}  // namespace SSDUI::Platform