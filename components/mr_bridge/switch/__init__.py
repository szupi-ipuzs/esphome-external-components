import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import switch
from esphome.const import (
   CONF_ID
)

from .. import mr_bridge_ns

CONF_FWD_PIN = "pin_forward"
CONF_REV_PIN = "pin_reverse"
CONF_PULSE_LEN = "pulse_length"

MRBridgeSwitch = mr_bridge_ns.class_('MRBridgeSwitch', switch.Switch, cg.Component)

CONFIG_SCHEMA = switch.SWITCH_SCHEMA.extend(
   {
      cv.GenerateID(CONF_ID): cv.declare_id(MRBridgeSwitch),
      cv.Required(CONF_FWD_PIN): pins.gpio_output_pin_schema,
      cv.Required(CONF_REV_PIN): pins.gpio_output_pin_schema,
      cv.Required(CONF_PULSE_LEN): cv.int_range(min=5, max=10000),
   }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
   var = cg.new_Pvariable(config[CONF_ID])
   await cg.register_component(var, config)
   await switch.register_switch(var, config)
   fwd_pin = await cg.gpio_pin_expression(config[CONF_FWD_PIN])
   rev_pin = await cg.gpio_pin_expression(config[CONF_REV_PIN])
   cg.add(var.set_mrbridge_config(fwd_pin, rev_pin, config[CONF_PULSE_LEN]))
