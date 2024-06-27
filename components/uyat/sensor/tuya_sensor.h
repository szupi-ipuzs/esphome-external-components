#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uyat/uyat.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace uyat {

class UyatSensor : public sensor::Sensor, public Component {
 public:
  void setup() override;
  void dump_config() override;
  void set_sensor_id(uint8_t sensor_id) { this->sensor_id_ = sensor_id; }

  void set_uyat_parent(Uyat *parent) { this->parent_ = parent; }

 protected:
  Uyat *parent_;
  uint8_t sensor_id_{0};
};

}  // namespace uyat
}  // namespace esphome
