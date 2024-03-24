#include "ssdui/render/default.hh"

#include <cstdint>

#include "HardwareSerial.h"

#ifdef ARDUINO

#include <cstddef>
#include <numeric>

namespace ssdui::render {

std::size_t DefaultRenderer::command(const std::vector<uint8_t>& data) {
  std::lock_guard<std::mutex> lock(m_mtx);

  std::size_t bytes_sent = 0;

  while (bytes_sent < data.size()) {
    m_wire->beginTransmission(m_address);
    m_wire->write(COMMAND_PREFIX);

    auto bytes_send_once_round =
        m_wire->write(data.data() + bytes_sent, data.size() - bytes_sent);

    if (m_wire->endTransmission() != ESP_OK) {
      break;
    }
    bytes_sent += bytes_send_once_round;
  }

  return bytes_sent;
}

std::size_t DefaultRenderer::data(const std::vector<uint8_t>& data) {
  std::lock_guard<std::mutex> lock(m_mtx);

  std::size_t bytes_sent = 0;

  while (bytes_sent < data.size()) {
    m_wire->beginTransmission(m_address);
    m_wire->write(DATA_PREFIX);

    auto bytes_send_once_round =
        m_wire->write(data.data() + bytes_sent, data.size() - bytes_sent);

    if (m_wire->endTransmission() != ESP_OK) {
      break;
    }
    bytes_sent += bytes_send_once_round;
  }

  return bytes_sent;
}

}  // namespace ssdui::render

#endif
