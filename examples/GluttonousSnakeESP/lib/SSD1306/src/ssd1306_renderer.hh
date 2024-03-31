#pragma once

#include <Wire.h>

#include <mutex>
#include <ssdui.hh>

namespace SSD1306 {

class Renderer {
 public:
  static constexpr uint8_t COMMAND_PREFIX = 0x00;
  static constexpr uint8_t DATA_PREFIX = 0x40;

 private:
  TwoWire* wire_;
  uint8_t address_;

  uint8_t sda_;
  uint8_t scl_;

  uint32_t frequency_;

  std::mutex mtx_{};

 public:
  Renderer(TwoWire* wire, uint8_t address, uint8_t sda, uint8_t scl,
           uint32_t frequency);
  Renderer(TwoWire* wire, uint8_t address, uint8_t sda, uint8_t scl);
  Renderer(TwoWire* wire, uint8_t address);

  ~Renderer();

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  std::size_t command(std::span<std::byte> data);
  std::size_t data(std::span<std::byte> data);
};

}  // namespace SSD1306