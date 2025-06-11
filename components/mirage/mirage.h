#pragma once

#include "esphome/components/climate_ir/climate_ir.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/remote_transmitter/remote_transmitter.h"

namespace esphome {
namespace mirage {

class MirageClimate : public climate_ir::ClimateIR {
 public:
  explicit MirageClimate() : climate_ir::ClimateIR(16.0f, 32.0f, 1.0f) {}

  void set_transmitter(remote_transmitter::RemoteTransmitterComponent *transmitter) {
    transmitter_ = transmitter;
  }

  void set_sensor(sensor::Sensor *sensor) {
    sensor_ = sensor;
  }

  void setup() override;

  climate::ClimateTraits traits() override;

  void control(const climate::ClimateCall &call) override;

 protected:
  void transmit_state() override;
  bool on_receive(remote_base::RemoteReceiveData data);

  remote_transmitter::RemoteTransmitterComponent *transmitter_{nullptr};
  sensor::Sensor *sensor_{nullptr};
  uint32_t last_transmit_time_{0};
};

}  // namespace mirage
}  // namespace esphome
