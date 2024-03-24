#pragma once

#include <type_traits>
namespace ssdui::common {

namespace _helper {

template <typename T, typename U>
concept SameHelper = std::is_same_v<T, U>;

}  // namespace _helper

/**
 * @brief Concept to check if two types are the same
 */
template <typename T, typename U>
concept SameAs = _helper::SameHelper<T, U> && _helper::SameHelper<U, T>;

}  // namespace ssdui::common
