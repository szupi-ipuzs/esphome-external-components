#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uyat/uyat.h"
#include "esphome/components/light/light_output.h"

namespace esphome {
namespace uyat {

enum UyatColorType {
  RGB,
  HSV,
  RGBHSV,
};

class UyatLight : public Component, public light::LightOutput {
 public:
  void setup() override;
  void dump_config() override;
  void set_dimmer_id(uint8_t dimmer_id) { this->dimmer_id_ = dimmer_id; }
  void set_min_value_datapoint_id(uint8_t min_value_datapoint_id) {
    this->min_value_datapoint_id_ = min_value_datapoint_id;
  }
  void set_switch_id(uint8_t switch_id) { this->switch_id_ = switch_id; }
  void set_color_id(uint8_t color_id) { this->color_id_ = color_id; }
  void set_color_type(UyatColorType color_type) { this->color_type_ = color_type; }
  void set_color_temperature_id(uint8_t color_temperature_id) { this->color_temperature_id_ = color_temperature_id; }
  void set_color_temperature_invert(bool color_temperature_invert) {
    this->color_temperature_invert_ = color_temperature_invert;
  }
  void set_uyat_parent(Uyat *parent) { this->parent_ = parent; }
  void set_min_value(uint32_t min_value) { min_value_ = min_value; }
  void set_max_value(uint32_t max_value) { max_value_ = max_value; }
  void set_color_temperature_max_value(uint32_t color_temperature_max_value) {
    this->color_temperature_max_value_ = color_temperature_max_value;
  }
  void set_cold_white_temperature(float cold_white_temperature) {
    this->cold_white_temperature_ = cold_white_temperature;
  }
  void set_warm_white_temperature(float warm_white_temperature) {
    this->warm_white_temperature_ = warm_white_temperature;
  }
  void set_color_interlock(bool color_interlock) { color_interlock_ = color_interlock; }

  light::LightTraits get_traits() override;
  void setup_state(light::LightState *state) override;
  void write_state(light::LightState *state) override;

 protected:
  void update_dimmer_(uint32_t value);
  void update_switch_(uint32_t value);

  Uyat *parent_;
  optional<uint8_t> dimmer_id_{};
  optional<uint8_t> min_value_datapoint_id_{};
  optional<uint8_t> switch_id_{};
  optional<uint8_t> color_id_{};
  optional<UyatColorType> color_type_{};
  optional<uint8_t> color_temperature_id_{};
  uint32_t min_value_ = 0;
  uint32_t max_value_ = 255;
  uint32_t color_temperature_max_value_ = 255;
  float cold_white_temperature_;
  float warm_white_temperature_;
  bool color_temperature_invert_{false};
  bool color_interlock_{false};
  light::LightState *state_{nullptr};
};

}  // namespace uyat
}  // namespace esphome
