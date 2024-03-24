#pragma once

#ifdef ARDUINO

#include <Wire.h>

#include <cstdint>
#include <mutex>

#include "ssdui/render/concept.hh"

namespace ssdui::render {

class DefaultRenderer {
 public:
  inline static constexpr uint8_t COMMAND_PREFIX = 0x00;
  inline static constexpr uint8_t DATA_PREFIX = 0x40;

 private:
  std::mutex m_mtx{};
  TwoWire* m_wire;
  uint8_t m_address;

 public:
  DefaultRenderer(TwoWire* wire, uint8_t address)
      : m_wire(wire), m_address(address) {
    wire->begin();
  }

  ~DefaultRenderer() { m_wire->end(); }

  std::size_t command(const std::vector<uint8_t>& data);
  std::size_t data(const std::vector<uint8_t>& data);
};

}  // namespace ssdui::render

#endif
