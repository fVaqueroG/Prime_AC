#include "mirage.h"
#include "esphome/core/log.h"

namespace esphome {
namespace mirage {

static const char *const TAG = "mirage.climate";

void MirageClimate::setup() {
  climate_ir::ClimateIR::setup();
  if (this->sensor_) {
    this->sensor_->add_on_state_callback([this](float state) {
      this->current_temperature = state;
      this->publish_state();
    });
  }
  this->mode = climate::CLIMATE_MODE_OFF;
  this->target_temperature = 24.0f;
  this->publish_state();
}

climate::ClimateTraits MirageClimate::traits() {
  auto traits = climate::ClimateTraits();
  traits.set_supports_current_temperature(this->sensor_ != nullptr);
  traits.set_supported_modes({
      climate::CLIMATE_MODE_OFF,
      climate::CLIMATE_MODE_HEAT,
      climate::CLIMATE_MODE_COOL,
      climate::CLIMATE_MODE_DRY,
      climate::CLIMATE_MODE_FAN_ONLY,
      climate::CLIMATE_MODE_HEAT_COOL
  });
  return traits;
}

void MirageClimate::control(const climate::ClimateCall &call) {
  if (call.get_mode().has_value())
    this->mode = *call.get_mode();
  if (call.get_target_temperature().has_value())
    this->target_temperature = *call.get_target_temperature();
  if (call.get_fan_mode().has_value())
    this->fan_mode = *call.get_fan_mode();
  if (call.get_swing_mode().has_value())
    this->swing_mode = *call.get_swing_mode();
  if (call.get_preset().has_value())
    this->preset = *call.get_preset();

  this->transmit_state();
  this->publish_state();
}

void MirageClimate::transmit_state() {
  // Your IR transmission implementation here
  ESP_LOGD(TAG, "Transmitting state");
}

bool MirageClimate::on_receive(remote_base::RemoteReceiveData data) {
  // Your IR reception implementation here
  return false;
}

}  // namespace mirage
}  // namespace esphome
