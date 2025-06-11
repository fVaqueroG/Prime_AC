#include "mirage.h"
#include "esphome/core/log.h"

namespace esphome {
namespace mirage {

static const char *const TAG = "mirage.climate";

void MirageClimate::setup() {
  climate_ir::ClimateIR::setup();
  
  // Initialize with default values
  this->mode = climate::CLIMATE_MODE_OFF;
  this->target_temperature = 24.0f;
  this->fan_mode = climate::CLIMATE_FAN_AUTO;
  this->swing_mode = climate::CLIMATE_SWING_OFF;
  
  // Set up sensor if available
  if (this->sensor_) {
    this->sensor_->add_on_state_callback([this](float state) {
      this->current_temperature = state;
      this->publish_state();
    });
    this->current_temperature = this->sensor_->state;
  }
  
  // Publish initial state
  this->publish_state();
  
  ESP_LOGI(TAG, "Mirage climate initialized");
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
  
  traits.set_supported_fan_modes({
      climate::CLIMATE_FAN_AUTO,
      climate::CLIMATE_FAN_LOW,
      climate::CLIMATE_FAN_MEDIUM,
      climate::CLIMATE_FAN_HIGH
  });
  
  traits.set_supported_swing_modes({
      climate::CLIMATE_SWING_OFF,
      climate::CLIMATE_SWING_VERTICAL,
      climate::CLIMATE_SWING_HORIZONTAL,
      climate::CLIMATE_SWING_BOTH
  });
  
  return traits;
}

void MirageClimate::control(const climate::ClimateCall &call) {
  if (call.get_mode().has_value()) {
    this->mode = *call.get_mode();
  }
  if (call.get_target_temperature().has_value()) {
    this->target_temperature = *call.get_target_temperature();
  }
  if (call.get_fan_mode().has_value()) {
    this->fan_mode = *call.get_fan_mode();
  }
  if (call.get_swing_mode().has_value()) {
    this->swing_mode = *call.get_swing_mode();
  }
  
  this->transmit_state();
  this->publish_state();
}

void MirageClimate::transmit_state() {
  ESP_LOGD(TAG, "Transmitting climate state");
  // Your IR transmission code here
}

}  // namespace mirage
}  // namespace esphome
