#pragma once

// 因为当前编译环境不存在 <concepts>, 用该文件显式扩充 <concepts> 的定义
// 只定义 std::same_as
//  TODO(dessera): 后续会使用 GCC >= 12 以引入 <concepts>

#include <type_traits>

namespace std {

template <typename T, typename U>
concept same_as = std::is_same_v<T, U> && std::is_same_v<U, T>;

}  // namespace std