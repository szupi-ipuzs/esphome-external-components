#include "esphome/core/log.h"
#include "uyat_binary_sensor.h"

namespace esphome {
namespace uyat {

static const char *const TAG = "uyat.binary_sensor";

void UyatBinarySensor::setup() {
  this->parent_->register_listener(this->sensor_id_, [this](const UyatDatapoint &datapoint) {
    ESP_LOGV(TAG, "MCU reported binary sensor %u is: %s", datapoint.id, ONOFF(datapoint.value_bool));
    this->publish_state(datapoint.value_bool);
  });
}

void UyatBinarySensor::dump_config() {
  ESP_LOGCONFIG(TAG, "Uyat Binary Sensor:");
  ESP_LOGCONFIG(TAG, "  Binary Sensor has datapoint ID %u", this->sensor_id_);
}

}  // namespace uyat
}  // namespace esphome
