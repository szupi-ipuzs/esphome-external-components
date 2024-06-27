#include "uyat_text_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace uyat {

static const char *const TAG = "uyat.text_sensor";

void UyatTextSensor::setup() {
  this->parent_->register_listener(this->sensor_id_, [this](const UyatDatapoint &datapoint) {
    switch (datapoint.type) {
      case UyatDatapointType::STRING:
        ESP_LOGD(TAG, "MCU reported text sensor %u is: %s", datapoint.id, datapoint.value_string.c_str());
        this->publish_state(datapoint.value_string);
        break;
      case UyatDatapointType::RAW: {
        std::string data = format_hex_pretty(datapoint.value_raw);
        ESP_LOGD(TAG, "MCU reported text sensor %u is: %s", datapoint.id, data.c_str());
        this->publish_state(data);
        break;
      }
      case UyatDatapointType::ENUM: {
        std::string data = to_string(datapoint.value_enum);
        ESP_LOGD(TAG, "MCU reported text sensor %u is: %s", datapoint.id, data.c_str());
        this->publish_state(data);
        break;
      }
      default:
        ESP_LOGW(TAG, "Unsupported data type for uyat text sensor %u: %#02hhX", datapoint.id, (uint8_t) datapoint.type);
        break;
    }
  });
}

void UyatTextSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "Uyat Text Sensor:");
  ESP_LOGCONFIG(TAG, "  Text Sensor has datapoint ID %u", this->sensor_id_);
}

}  // namespace uyat
}  // namespace esphome
