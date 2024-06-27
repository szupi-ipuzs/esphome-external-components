#include "esphome/core/log.h"

#include "automation.h"

static const char *const TAG = "uyat.automation";

namespace esphome {
namespace uyat {

void check_expected_datapoint(const UyatDatapoint &dp, UyatDatapointType expected) {
  if (dp.type != expected) {
    ESP_LOGW(TAG, "Uyat sensor %u expected datapoint type %#02hhX but got %#02hhX", dp.id,
             static_cast<uint8_t>(expected), static_cast<uint8_t>(dp.type));
  }
}

UyatRawDatapointUpdateTrigger::UyatRawDatapointUpdateTrigger(Uyat *parent, uint8_t sensor_id) {
  parent->register_listener(sensor_id, [this](const UyatDatapoint &dp) {
    check_expected_datapoint(dp, UyatDatapointType::RAW);
    this->trigger(dp.value_raw);
  });
}

UyatBoolDatapointUpdateTrigger::UyatBoolDatapointUpdateTrigger(Uyat *parent, uint8_t sensor_id) {
  parent->register_listener(sensor_id, [this](const UyatDatapoint &dp) {
    check_expected_datapoint(dp, UyatDatapointType::BOOLEAN);
    this->trigger(dp.value_bool);
  });
}

UyatIntDatapointUpdateTrigger::UyatIntDatapointUpdateTrigger(Uyat *parent, uint8_t sensor_id) {
  parent->register_listener(sensor_id, [this](const UyatDatapoint &dp) {
    check_expected_datapoint(dp, UyatDatapointType::INTEGER);
    this->trigger(dp.value_int);
  });
}

UyatUIntDatapointUpdateTrigger::UyatUIntDatapointUpdateTrigger(Uyat *parent, uint8_t sensor_id) {
  parent->register_listener(sensor_id, [this](const UyatDatapoint &dp) {
    check_expected_datapoint(dp, UyatDatapointType::INTEGER);
    this->trigger(dp.value_uint);
  });
}

UyatStringDatapointUpdateTrigger::UyatStringDatapointUpdateTrigger(Uyat *parent, uint8_t sensor_id) {
  parent->register_listener(sensor_id, [this](const UyatDatapoint &dp) {
    check_expected_datapoint(dp, UyatDatapointType::STRING);
    this->trigger(dp.value_string);
  });
}

UyatEnumDatapointUpdateTrigger::UyatEnumDatapointUpdateTrigger(Uyat *parent, uint8_t sensor_id) {
  parent->register_listener(sensor_id, [this](const UyatDatapoint &dp) {
    check_expected_datapoint(dp, UyatDatapointType::ENUM);
    this->trigger(dp.value_enum);
  });
}

UyatBitmaskDatapointUpdateTrigger::UyatBitmaskDatapointUpdateTrigger(Uyat *parent, uint8_t sensor_id) {
  parent->register_listener(sensor_id, [this](const UyatDatapoint &dp) {
    check_expected_datapoint(dp, UyatDatapointType::BITMASK);
    this->trigger(dp.value_bitmask);
  });
}

}  // namespace uyat
}  // namespace esphome
