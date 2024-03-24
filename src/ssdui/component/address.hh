#pragma once

#include <cstdint>
#include <memory>

#include "ssdui/context/config.hh"
#include "ssdui/context/core.hh"

namespace ssdui::command::address {

/**
 * @brief 设置起始列，用于页寻址模式
 *
 * @tparam Rnd 渲染器类型
 */
template <typename Rnd>
  requires render::IsRenderer<Rnd>
class SetStartColumn {
 public:
  static inline constexpr uint8_t COMMAND_SET_START_COLUMN_LOW = 0x00;
  static inline constexpr uint8_t COMMAND_SET_START_COLUMN_HIGH = 0x10;
  static inline constexpr uint8_t COLUMN_MASK = 0x0F;

 private:
  uint8_t m_column;

 public:
  explicit SetStartColumn(uint8_t column) : m_column(column) {}

  void operator()(context::Context<Rnd>* ctx) const {
    const auto& config = ctx->config();

    if (config.addressing_mode != context::AddressMode::PAGE) {
      return;
    }

    if (m_column >= config.width) {
      return;
    }

    ctx->renderer()->command({COMMAND_SET_START_COLUMN_LOW,
                              static_cast<uint8_t>(m_column & COLUMN_MASK),
                              COMMAND_SET_START_COLUMN_HIGH,
                              m_column >> static_cast<uint8_t>(4)});
  }
};

/**
 * @brief 设置起始页，用于页寻址模式
 *
 * @tparam Rnd 渲染器类型
 */
template <typename Rnd>
  requires render::IsRenderer<Rnd>
class SetStartPage {
 public:
  static inline constexpr uint8_t COMMAND_SET_START_PAGE = 0xB0;

 private:
  uint8_t m_page;

 public:
  explicit SetStartPage(uint8_t page) : m_page(page) {}

  void operator()(context::Context<Rnd>* ctx) const {
    const auto& config = ctx->config();

    if (config.addressing_mode != context::AddressMode::PAGE) {
      return;
    }

    if (m_page >= config.page) {
      return;
    }

    ctx->renderer()->command({COMMAND_SET_START_PAGE | m_page});
  }
};

/**
 * @brief 设置寻址模式
 *
 * @tparam Rnd 渲染器类型
 */
template <typename Rnd>
  requires render::IsRenderer<Rnd>
class SetAddressingMode {
 public:
  static inline constexpr uint8_t COMMAND_SET_ADDRESSING_MODE = 0x20;

 private:
  context::AddressMode m_mode;

 public:
  explicit SetAddressingMode(context::AddressMode mode) : m_mode(mode) {}

  void operator()(context::Context<Rnd>* ctx) const {
    ctx->renderer()->command(
        {COMMAND_SET_ADDRESSING_MODE, static_cast<uint8_t>(m_mode)});
  }
};

/**
 * @brief 设置列地址范围，用于水平寻址模式和垂直寻址模式
 *
 * @tparam Rnd 渲染器类型
 */
template <typename Rnd>
  requires render::IsRenderer<Rnd>
class SetColumnAddress {
 public:
  static inline constexpr uint8_t COMMAND_SET_COLUMN_ADDRESS = 0x21;

 private:
  uint8_t m_start_column;
  uint8_t m_end_column;

 public:
  explicit SetColumnAddress(uint8_t start_column, uint8_t end_column)
      : m_start_column(start_column), m_end_column(end_column) {}

  void operator()(context::Context<Rnd>* ctx) const {
    const auto& config = ctx->config();

    if (config.addressing_mode == context::AddressMode::PAGE) {
      // downcast to SetStartColumn
      ctx->run(SetStartColumn<Rnd>(m_start_column));
    }

    if (m_start_column >= config.width || m_end_column >= config.width) {
      return;
    }

    ctx->renderer()->command(
        {COMMAND_SET_COLUMN_ADDRESS, m_start_column, m_end_column});
  }
};

/**
 * @brief 设置页地址范围，用于水平寻址模式和垂直寻址模式
 *
 * @tparam Rnd 渲染器类型
 */
template <typename Rnd>
  requires render::IsRenderer<Rnd>
class SetPageAddress {
 public:
  static inline constexpr uint8_t COMMAND_SET_PAGE_ADDRESS = 0x22;

 private:
  uint8_t m_start_page;
  uint8_t m_end_page;

 public:
  explicit SetPageAddress(uint8_t start_page, uint8_t end_page)
      : m_start_page(start_page), m_end_page(end_page) {}

  void operator()(context::Context<Rnd>* ctx) const {
    const auto& config = ctx->config();

    if (config.addressing_mode == context::AddressMode::PAGE) {
      // downcast to SetStartPage
      ctx->run(SetStartPage<Rnd>(m_start_page));
    }

    if (m_start_page >= config.page || m_end_page >= config.page) {
      return;
    }

    ctx->renderer()->command(
        {COMMAND_SET_PAGE_ADDRESS, m_start_page, m_end_page});
  }
};

}  // namespace ssdui::command::address