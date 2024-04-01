#pragma once

#include <cstdint>
#include <ssdui/geometry/point.hh>

enum class GlutState {
  Ready,
  Running,
  Failed,
};

struct GlutStore {
  GlutState state{GlutState::Ready};
  SSDUI::Geometry::Point<int32_t> food{};
  int32_t score{0};
};