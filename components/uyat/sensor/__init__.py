from esphome.components import sensor
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_ID, CONF_SENSOR_DATAPOINT
from .. import uyat_ns, CONF_UYAT_ID, Uyat

DEPENDENCIES = ["uyat"]
CODEOWNERS = ["@jesserockz"]

UyatSensor = uyat_ns.class_("UyatSensor", sensor.Sensor, cg.Component)

CONFIG_SCHEMA = (
    sensor.sensor_schema(UyatSensor)
    .extend(
        {
            cv.GenerateID(CONF_UYAT_ID): cv.use_id(Uyat),
            cv.Required(CONF_SENSOR_DATAPOINT): cv.uint8_t,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)

    paren = await cg.get_variable(config[CONF_UYAT_ID])
    cg.add(var.set_uyat_parent(paren))

    cg.add(var.set_sensor_id(config[CONF_SENSOR_DATAPOINT]))
