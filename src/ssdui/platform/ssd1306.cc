#include "ssdui/platform/ssd1306.hh"

namespace SSDUI::Platform {

SSD1306::Renderer::Renderer(TwoWire* wire, uint8_t address, uint8_t sda,
                            uint8_t scl, uint32_t frequency)
    : wire_(wire),
      address_(address),
      sda_(sda),
      scl_(scl),
      frequency_(frequency) {
  wire_->begin(sda_, scl_, frequency_);
}

SSD1306::Renderer::Renderer(TwoWire* wire, uint8_t address, uint8_t sda,
                            uint8_t scl)
    : SSD1306::Renderer::Renderer(wire, address, sda, scl, 400000) {}

SSD1306::Renderer::Renderer(TwoWire* wire, uint8_t address)
    : Renderer(wire, address, 4, 5) {}

SSD1306::Renderer::~Renderer() { wire_->end(); }

std::size_t SSD1306::Renderer::command(std::span<uint8_t> data) {
  std::lock_guard<std::mutex> lock(mtx_);

  std::size_t bytes_sent = 0;

  while (bytes_sent < data.size()) {
    wire_->beginTransmission(address_);
    wire_->write(COMMAND_PREFIX);

    auto bytes_send_once_round =
        wire_->write(data.data() + bytes_sent, data.size() - bytes_sent);

    if (wire_->endTransmission() != 0) {
      break;
    }
    bytes_sent += bytes_send_once_round;
  }

  return bytes_sent;
}

std::size_t SSD1306::Renderer::data(std::span<uint8_t> data) {
  std::lock_guard<std::mutex> lock(mtx_);

  std::size_t bytes_sent = 0;

  while (bytes_sent < data.size()) {
    wire_->beginTransmission(address_);
    wire_->write(DATA_PREFIX);

    auto bytes_send_once_round =
        wire_->write(data.data() + bytes_sent, data.size() - bytes_sent);

    if (wire_->endTransmission() != 0) {
      break;
    }
    bytes_sent += bytes_send_once_round;
  }

  return bytes_sent;
}

}  // namespace SSDUI::Platform