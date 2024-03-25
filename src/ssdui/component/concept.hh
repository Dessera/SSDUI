#pragma once

#include <memory>
#include <type_traits>

#include "ssdui/context/core.hh"

namespace ssdui::component {

template <typename Cmp, typename Rnd>
concept IsComponent = requires(Cmp comp, context::Context<Rnd> *ctx) {
  { comp(ctx) };
};

// maybe another wat to impl
template <typename Cmp, typename Rnd, typename Event>
concept IsEventComponent = requires(Cmp comp) {
  requires std::is_enum_v<Event>;
  requires IsComponent<Cmp, Rnd>;
  { comp.on_event(std::shared_ptr<context::Context<Rnd>>{}, Event{}) };
};

}  // namespace ssdui::component
