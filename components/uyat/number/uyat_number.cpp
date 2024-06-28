#include "esphome/core/log.h"
#include "uyat_number.h"

namespace esphome {
namespace uyat {

static const char *const TAG = "uyat.number";

void UyatNumber::setup() {
  this->parent_->register_listener(this->number_id_, [this](const UyatDatapoint &datapoint) {
    if (datapoint.type == UyatDatapointType::INTEGER) {
      ESP_LOGV(TAG, "MCU reported number %u is: %d", datapoint.id, datapoint.value_int);
      this->publish_state(datapoint.value_int / multiply_by_);
    } else if (datapoint.type == UyatDatapointType::ENUM) {
      ESP_LOGV(TAG, "MCU reported number %u is: %u", datapoint.id, datapoint.value_enum);
      this->publish_state(datapoint.value_enum);
    }
    if ((this->type_) && (this->type_ != datapoint.type))
    {
       ESP_LOGW(TAG, "Reported type (%d) different than previously set (%d)!", datapoint.type, this->type_);
    }
    this->type_ = datapoint.type;
  });
}

void UyatNumber::control(float value) {
  ESP_LOGV(TAG, "Setting number %u: %f", this->number_id_, value);
  if (this->type_ == UyatDatapointType::INTEGER) {
    int integer_value = lround(value * multiply_by_);
    this->parent_->set_integer_datapoint_value(this->number_id_, integer_value);
  } else if (this->type_ == UyatDatapointType::ENUM) {
    this->parent_->set_enum_datapoint_value(this->number_id_, value);
  }
  this->publish_state(value);
}

void UyatNumber::dump_config() {
  LOG_NUMBER("", "Uyat Number", this);
  ESP_LOGCONFIG(TAG, "  Number has datapoint ID %u", this->number_id_);
}

}  // namespace uyat
}  // namespace esphome
