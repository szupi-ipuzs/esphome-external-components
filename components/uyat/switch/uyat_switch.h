#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uyat/uyat.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace uyat {

class UyatSwitch : public switch_::Switch, public Component {
 public:
  void setup() override;
  void dump_config() override;
  void set_switch_id(uint8_t switch_id) { this->switch_id_ = switch_id; }

  void set_uyat_parent(Uyat *parent) { this->parent_ = parent; }

 protected:
  void write_state(bool state) override;

  Uyat *parent_;
  uint8_t switch_id_{0};
};

}  // namespace uyat
}  // namespace esphome
