#include "esphome/core/log.h"
#include "uyat_sensor.h"
#include <cinttypes>

namespace esphome {
namespace uyat {

static const char *const TAG = "uyat.sensor";

void UyatSensor::setup() {
  this->parent_->register_listener(this->sensor_id_, [this](const UyatDatapoint &datapoint) {
    if (datapoint.type == UyatDatapointType::BOOLEAN) {
      ESP_LOGV(TAG, "MCU reported sensor %u is: %s", datapoint.id, ONOFF(datapoint.value_bool));
      this->publish_state(datapoint.value_bool);
    } else if (datapoint.type == UyatDatapointType::INTEGER) {
      ESP_LOGV(TAG, "MCU reported sensor %u is: %d", datapoint.id, datapoint.value_int);
      this->publish_state(datapoint.value_int);
    } else if (datapoint.type == UyatDatapointType::ENUM) {
      ESP_LOGV(TAG, "MCU reported sensor %u is: %u", datapoint.id, datapoint.value_enum);
      this->publish_state(datapoint.value_enum);
    } else if (datapoint.type == UyatDatapointType::BITMASK) {
      ESP_LOGV(TAG, "MCU reported sensor %u is: %" PRIx32, datapoint.id, datapoint.value_bitmask);
      this->publish_state(datapoint.value_bitmask);
    }
  });
}

void UyatSensor::dump_config() {
  LOG_SENSOR("", "Uyat Sensor", this);
  ESP_LOGCONFIG(TAG, "  Sensor has datapoint ID %u", this->sensor_id_);
}

}  // namespace uyat
}  // namespace esphome
