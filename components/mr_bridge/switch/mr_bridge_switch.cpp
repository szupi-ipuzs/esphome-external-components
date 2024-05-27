#include "mr_bridge_switch.h"
#include "esphome/core/log.h"

namespace esphome{

namespace mr_bridge{

static const char *TAG = "mr_bridge.switch";

void MRBridgeSwitch::set_mrbridge_config(InternalGPIOPin *pin_fwd, InternalGPIOPin *pin_rev, int pulse_len)
{
   m_pin_fwd = pin_fwd;
   m_pin_rev = pin_rev;
   m_pulse_len = pulse_len;
   m_pulse_cnt = 0;
}

void MRBridgeSwitch::setup()
{
   this->m_pin_fwd->setup();
   this->m_pin_rev->setup();
   this->m_pin_fwd->digital_write(false);
   this->m_pin_rev->digital_write(false);

   bool initial_state = this->get_initial_state_with_restore_mode().value_or(false);
   // write state before setup
   if (initial_state) {
      this->turn_on();
   } else {
      this->turn_off();
   }
}

void MRBridgeSwitch::write_state(bool state)
{
   ESP_LOGCONFIG(TAG, "Requesting new state: %d", state);
   m_new_state = state;
}

void MRBridgeSwitch::dump_config()
{
   ESP_LOGCONFIG(TAG, "FWD pin is '%d'...", this->m_pin_fwd->get_pin());
   ESP_LOGCONFIG(TAG, "REV pin is '%d'...", this->m_pin_rev->get_pin());
   ESP_LOGCONFIG(TAG, "Pulse length is '%d'...", this->m_pulse_len);
}

void MRBridgeSwitch::loop()
{
   if (m_pulse_cnt > 0)
   {
      --m_pulse_cnt;
      if (m_pulse_cnt == 0)
      {
         ESP_LOGCONFIG(TAG, "Pulse Complete. HOLD");
         this->m_pin_fwd->digital_write(false);
         this->m_pin_rev->digital_write(false);
         this->publish_state(m_current_state);
      }
   }
   else
   if (m_current_state != m_new_state)
   {
      if (m_new_state)
      {
         ESP_LOGCONFIG(TAG, "Forward pulse initiated");
         this->m_pin_fwd->digital_write(true);
         m_current_state = m_new_state;
         m_pulse_cnt = m_pulse_len;
      }
      else
      {
         ESP_LOGCONFIG(TAG, "Reverse pulse initiated");
         this->m_pin_rev->digital_write(true);
         m_current_state = m_new_state;
         m_pulse_cnt = m_pulse_len;
      }
   }
}

}}
