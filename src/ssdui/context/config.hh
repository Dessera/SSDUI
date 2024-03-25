#pragma once

#include <cstddef>
#include <cstdint>

namespace ssdui::context {

/**
 * @brief 内存寻址模式
 */
enum class AddressMode : uint8_t {
  HORIZONTAL = 0x00,
  VERTICAL = 0x01,
  PAGE = 0x02,
};

/**
 * @brief 硬件接口配置
 */
enum class ComPinsConfig : uint8_t {
  SEQUENTIAL = 0x02,
  SEQUENTIAL_REMAPPED = 0x22,
  ALTERNATIVE = 0x12,
  ALTERNATIVE_REMAPPED = 0x32,
};

/**
 * @brief 上下文配置对象
 */
struct Config {
  /**
   * @brief 默认显示宽度
   */
  static constexpr uint8_t DEFAULT_WIDTH = 128;

  /**
   * @brief 默认显示页数
   */
  static constexpr uint8_t DEFAULT_PAGE = 8;

  /**
   * @brief 默认页大小
   */
  static constexpr uint8_t DEFAULT_PAGE_SIZE = 8;

  /**
   * @brief 默认初始化起始页
   */
  static constexpr uint8_t DEFAULT_INIT_PAGE_START = 0;

  /**
   * @brief 默认初始化结束页
   */
  static constexpr uint8_t DEFAULT_INIT_PAGE_END = DEFAULT_PAGE - 1;

  /**
   * @brief 默认初始化起始列
   */
  static constexpr uint8_t DEFAULT_INIT_COLUMN_START = 0;

  /**
   * @brief 默认初始化结束列
   */
  static constexpr uint8_t DEFAULT_INIT_COLUMN_END = DEFAULT_WIDTH - 1;

  /**
   * @brief 默认寻址模式
   */
  static constexpr AddressMode DEFAULT_ADDRESSING_MODE = AddressMode::PAGE;

  /**
   * @brief 默认时钟分频
   */
  static constexpr uint8_t DEFAULT_RATIO = 0x00;

  /**
   * @brief 默认振荡频率
   */
  static constexpr uint8_t DEFAULT_FREQUENCY = 0x08;

  /**
   * @brief 默认预充电周期1
   */
  static constexpr uint8_t DEFAULT_PRECHARGE_PHASE1 = 0x01;

  /**
   * @brief 默认预充电周期2
   */
  static constexpr uint8_t DEFAULT_PRECHARGE_PHASE2 = 0x0F;

  /**
   * @brief 默认VCOMH电压
   * TODO: Maybe refactor this to an enum
   */
  static constexpr uint8_t DEFAULT_VCOMH_LEVEL = 0x40;

  /**
   * @brief 默认对比度
   */
  static constexpr uint8_t DEFAULT_CONTRAST = 0x7F;

  /**
   * @brief 默认Com引脚配置
   */
  static constexpr ComPinsConfig DEFAULT_COM_PINS = ComPinsConfig::ALTERNATIVE;

  /**
   * @brief 默认显示全亮
   */
  static constexpr bool DEFAULT_ENTIRE_DISPLAY_ON = false;

  /**
   * @brief 默认显示反色
   */
  static constexpr bool DEFAULT_INVERSE_DISPLAY = false;

  /**
   * @brief 默认显示开关
   */
  static constexpr bool DEFAULT_DISPLAY_ON = true;

  /**
   * @brief 默认显示行
   */
  static constexpr uint8_t DEFAULT_START_LINE = 0;

  /**
   * @brief 默认左右翻转
   */
  static constexpr bool DEFAULT_HORIZONTAL_FLIP = false;

  /**
   * @brief 默认上下翻转
   */
  static constexpr bool DEFAULT_VERTICAL_FLIP = false;

  /**
   * @brief 默认显示偏移
   */
  static constexpr uint8_t DEFAULT_DISPLAY_OFFSET = 0;

  /**
   * @brief 默认通道数
   */
  static constexpr uint8_t DEFAULT_MULTIPLEX_RATIO =
      DEFAULT_PAGE_SIZE * DEFAULT_PAGE;

  /**
   * @brief 电荷泵使能
   */
  static constexpr bool DEFAULT_CHARGE_PUMP_ENABLE = true;

  /**
   * @brief 默认帧数
   */
  static constexpr uint8_t DEFAULT_FRAME_RATE = 30;

  /**
   * @brief OLED显示驱动显示宽度
   * @note 可以自定义，该条目实际上是软件限制
   */
  uint8_t width{DEFAULT_WIDTH};

  /**
   * @brief OLED显示驱动显示页数
   * @note 可以自定义，该条目实际上是软件限制
   */
  uint8_t page{DEFAULT_PAGE};

  /**
   * @brief OLED显示驱动寻址模式
   */
  AddressMode addressing_mode{DEFAULT_ADDRESSING_MODE};

  /**
   * @brief 显示的起始页
   */
  uint8_t page_start{DEFAULT_INIT_PAGE_START};

  /**
   * @brief 初始化显示的结束页
   * @note 在页寻址模式下，该条目无效
   */
  uint8_t page_end{DEFAULT_INIT_PAGE_END};

  /**
   * @brief 初始化显示的起始列
   */
  uint8_t column_start{DEFAULT_INIT_COLUMN_START};

  /**
   * @brief 初始化显示的结束列
   * @note 在页寻址模式下，该条目无效
   */
  uint8_t column_end{DEFAULT_INIT_COLUMN_END};

  /**
   * @brief 时钟分频
   */
  uint8_t clock_ratio{DEFAULT_RATIO};

  /**
   * @brief 振荡频率
   */
  uint8_t clock_frequency{DEFAULT_FREQUENCY};

  /**
   * @brief 预充电周期1
   */
  uint8_t precharge_phase1{DEFAULT_PRECHARGE_PHASE1};

  /**
   * @brief 预充电周期2
   */
  uint8_t precharge_phase2{DEFAULT_PRECHARGE_PHASE2};

  /**
   * @brief VCOMH电压
   */
  uint8_t vcomh_level{DEFAULT_VCOMH_LEVEL};

  /**
   * @brief 对比度
   */
  uint8_t contrast{DEFAULT_CONTRAST};

  /**
   * @brief 显示全亮
   */
  bool entire_display_on{DEFAULT_ENTIRE_DISPLAY_ON};

  /**
   * @brief 显示反色
   */
  bool inverse_display{DEFAULT_INVERSE_DISPLAY};

  /**
   * @brief 显示开关
   */
  bool display_on{DEFAULT_DISPLAY_ON};

  /**
   * @brief 初始显示行
   */
  uint8_t start_line{DEFAULT_START_LINE};

  /**
   * @brief 左右翻转
   */
  bool horizontal_flip{DEFAULT_HORIZONTAL_FLIP};

  /**
   * @brief 上下翻转
   */
  bool vertical_flip{DEFAULT_VERTICAL_FLIP};

  /**
   * @brief 启用MUX通道数
   * @note
   * OLED命令中，输入的ratio会被+1,这里自动处理的该情况，请填写正确的通道数。
   * 另外，该条目是硬件限制
   */
  uint8_t multiplex_ratio{DEFAULT_MULTIPLEX_RATIO};

  /**
   * @brief 显示偏移
   */
  uint8_t display_offset{DEFAULT_DISPLAY_OFFSET};

  /**
   * @brief COM引脚配置
   */
  ComPinsConfig com_pins{DEFAULT_COM_PINS};

  /**
   * @brief 电荷泵使能
   */
  bool charge_pump_enable{DEFAULT_CHARGE_PUMP_ENABLE};

  /**
   * @brief 帧数
   */
  uint8_t frame_rate{DEFAULT_FRAME_RATE};
} __attribute__((aligned(32)));

class RuntimeConfig {
 private:
  Config m_config{};

 public:
  RuntimeConfig() = default;

  RuntimeConfig(const RuntimeConfig&) = default;
  RuntimeConfig& operator=(const RuntimeConfig&) = default;
  RuntimeConfig(RuntimeConfig&&) = default;
  RuntimeConfig& operator=(RuntimeConfig&&) = default;

  ~RuntimeConfig() = default;

  explicit RuntimeConfig(const Config& config) : m_config(config) {}

  // all getters
  [[nodiscard]] std::uint8_t width() const { return m_config.width; }
  [[nodiscard]] std::uint8_t page() const { return m_config.page; }
  [[nodiscard]] std::size_t height() const {
    return m_config.page * Config::DEFAULT_PAGE_SIZE;
  }
  [[nodiscard]] AddressMode addressing_mode() const {
    return m_config.addressing_mode;
  }
  [[nodiscard]] std::uint8_t page_start() const { return m_config.page_start; }
  [[nodiscard]] std::uint8_t page_end() const { return m_config.page_end; }
  [[nodiscard]] std::uint8_t column_start() const {
    return m_config.column_start;
  }
  [[nodiscard]] std::uint8_t column_end() const { return m_config.column_end; }
  [[nodiscard]] std::uint8_t clock_ratio() const {
    return m_config.clock_ratio;
  }
  [[nodiscard]] std::uint8_t clock_frequency() const {
    return m_config.clock_frequency;
  }
  [[nodiscard]] std::uint8_t precharge_phase1() const {
    return m_config.precharge_phase1;
  }
  [[nodiscard]] std::uint8_t precharge_phase2() const {
    return m_config.precharge_phase2;
  }
  [[nodiscard]] std::uint8_t vcomh_level() const {
    return m_config.vcomh_level;
  }
  [[nodiscard]] std::uint8_t contrast() const { return m_config.contrast; }
  [[nodiscard]] bool entire_display_on() const {
    return m_config.entire_display_on;
  }
  [[nodiscard]] bool inverse_display() const {
    return m_config.inverse_display;
  }
  [[nodiscard]] bool display_on() const { return m_config.display_on; }
  [[nodiscard]] std::uint8_t start_line() const { return m_config.start_line; }
  [[nodiscard]] bool horizontal_flip() const {
    return m_config.horizontal_flip;
  }
  [[nodiscard]] bool vertical_flip() const { return m_config.vertical_flip; }
  [[nodiscard]] std::uint8_t multiplex_ratio() const {
    return m_config.multiplex_ratio;
  }
  [[nodiscard]] std::uint8_t display_offset() const {
    return m_config.display_offset;
  }
  [[nodiscard]] ComPinsConfig com_pins() const { return m_config.com_pins; }
  [[nodiscard]] bool charge_pump_enable() const {
    return m_config.charge_pump_enable;
  }
  [[nodiscard]] std::uint8_t frame_rate() const { return m_config.frame_rate; }

  // limited setters
  void set_contrast(std::uint8_t contrast) { m_config.contrast = contrast; }
  void set_display_on(bool display_on) { m_config.display_on = display_on; }
  void set_entire_display_on(bool entire_display_on) {
    m_config.entire_display_on = entire_display_on;
  }
  void set_inverse_display(bool inverse_display) {
    m_config.inverse_display = inverse_display;
  }
  void set_vertical_flip(bool vertical_flip) {
    m_config.vertical_flip = vertical_flip;
  }
  void set_horizontal_flip(bool horizontal_flip) {
    m_config.horizontal_flip = horizontal_flip;
  }
  void set_display_offset(std::uint8_t display_offset) {
    m_config.display_offset = display_offset;
  }
};

}  // namespace ssdui::context
