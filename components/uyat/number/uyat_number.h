#pragma once

#include "esphome/core/component.h"
#include "../uyat.h"
#include "esphome/components/number/number.h"
#include "esphome/core/optional.h"

namespace esphome {
namespace uyat {

class UyatNumber : public number::Number, public Component {
 public:
  void setup() override;
  void dump_config() override;
  void set_number_id(uint8_t number_id) { this->number_id_ = number_id; }
  void set_write_multiply(float factor) { multiply_by_ = factor; }
  void set_datapoint_type(UyatDatapointType type) { type_ = type; }

  void set_uyat_parent(Uyat *parent) { this->parent_ = parent; }

 protected:
  void control(float value) override;

  Uyat *parent_;
  uint8_t number_id_{0};
  float multiply_by_{1.0};
  optional<UyatDatapointType> type_{};
};

}  // namespace uyat
}  // namespace esphome
