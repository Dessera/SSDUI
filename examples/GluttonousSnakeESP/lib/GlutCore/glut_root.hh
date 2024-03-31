#pragma once

#include "ssd1306.hh"
#include "ssdui/context/component.hh"

class GlutRoot : public SSDUI::Context::BaseComponent<SSD1306::SSD1306> {
 private:
 public:
  GlutRoot() = default;
  virtual ~GlutRoot() = default;

  GlutRoot(const GlutRoot&) = delete;
  GlutRoot& operator=(const GlutRoot&) = delete;
  GlutRoot(GlutRoot&&) = delete;
  GlutRoot& operator=(GlutRoot&&) = delete;

  void operator()(SSDUI::Context::Context<SSD1306::SSD1306>* context) override {

  }
};
