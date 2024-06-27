#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "uyat.h"

#include <vector>

namespace esphome {
namespace uyat {

class UyatDatapointUpdateTrigger : public Trigger<UyatDatapoint> {
 public:
  explicit UyatDatapointUpdateTrigger(Uyat *parent, uint8_t sensor_id) {
    parent->register_listener(sensor_id, [this](const UyatDatapoint &dp) { this->trigger(dp); });
  }
};

class UyatRawDatapointUpdateTrigger : public Trigger<std::vector<uint8_t>> {
 public:
  explicit UyatRawDatapointUpdateTrigger(Uyat *parent, uint8_t sensor_id);
};

class UyatBoolDatapointUpdateTrigger : public Trigger<bool> {
 public:
  explicit UyatBoolDatapointUpdateTrigger(Uyat *parent, uint8_t sensor_id);
};

class UyatIntDatapointUpdateTrigger : public Trigger<int> {
 public:
  explicit UyatIntDatapointUpdateTrigger(Uyat *parent, uint8_t sensor_id);
};

class UyatUIntDatapointUpdateTrigger : public Trigger<uint32_t> {
 public:
  explicit UyatUIntDatapointUpdateTrigger(Uyat *parent, uint8_t sensor_id);
};

class UyatStringDatapointUpdateTrigger : public Trigger<std::string> {
 public:
  explicit UyatStringDatapointUpdateTrigger(Uyat *parent, uint8_t sensor_id);
};

class UyatEnumDatapointUpdateTrigger : public Trigger<uint8_t> {
 public:
  explicit UyatEnumDatapointUpdateTrigger(Uyat *parent, uint8_t sensor_id);
};

class UyatBitmaskDatapointUpdateTrigger : public Trigger<uint32_t> {
 public:
  explicit UyatBitmaskDatapointUpdateTrigger(Uyat *parent, uint8_t sensor_id);
};

}  // namespace uyat
}  // namespace esphome
