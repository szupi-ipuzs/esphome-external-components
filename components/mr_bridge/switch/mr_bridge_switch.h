#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"

namespace esphome{

namespace mr_bridge{

class MRBridgeSwitch: public switch_::Switch, public Component
{
public:

   void set_mrbridge_config(InternalGPIOPin *pin_fwd, InternalGPIOPin *pin_rev, int pulse_len);

   void setup() override;
   void write_state(bool state) override;
   void dump_config() override;
   void loop() override;

private:

   InternalGPIOPin *m_pin_fwd = nullptr;
   InternalGPIOPin *m_pin_rev = nullptr;
   int m_pulse_len = 5;
   int m_pulse_cnt = 0;
   bool m_current_state = false;
   int m_new_state = -1;
}

}}