from esphome.components import cover
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import (
    CONF_OUTPUT_ID,
    CONF_MIN_VALUE,
    CONF_MAX_VALUE,
    CONF_RESTORE_MODE,
)
from .. import uyat_ns, CONF_UYAT_ID, Uyat

DEPENDENCIES = ["uyat"]

CONF_CONTROL_DATAPOINT = "control_datapoint"
CONF_DIRECTION_DATAPOINT = "direction_datapoint"
CONF_POSITION_DATAPOINT = "position_datapoint"
CONF_POSITION_REPORT_DATAPOINT = "position_report_datapoint"
CONF_INVERT_POSITION = "invert_position"
CONF_INVERT_POSITION_REPORT = "invert_position_report"

UyatCover = uyat_ns.class_("UyatCover", cover.Cover, cg.Component)

UyatCoverRestoreMode = uyat_ns.enum("UyatCoverRestoreMode")
RESTORE_MODES = {
    "NO_RESTORE": UyatCoverRestoreMode.COVER_NO_RESTORE,
    "RESTORE": UyatCoverRestoreMode.COVER_RESTORE,
    "RESTORE_AND_CALL": UyatCoverRestoreMode.COVER_RESTORE_AND_CALL,
}


def validate_range(config):
    if config[CONF_MIN_VALUE] > config[CONF_MAX_VALUE]:
        raise cv.Invalid(
            f"min_value ({config[CONF_MIN_VALUE]}) cannot be greater than max_value ({config[CONF_MAX_VALUE]})"
        )
    return config


CONFIG_SCHEMA = cv.All(
    cover.COVER_SCHEMA.extend(
        {
            cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(UyatCover),
            cv.GenerateID(CONF_UYAT_ID): cv.use_id(Uyat),
            cv.Optional(CONF_CONTROL_DATAPOINT): cv.uint8_t,
            cv.Optional(CONF_DIRECTION_DATAPOINT): cv.uint8_t,
            cv.Required(CONF_POSITION_DATAPOINT): cv.uint8_t,
            cv.Optional(CONF_POSITION_REPORT_DATAPOINT): cv.uint8_t,
            cv.Optional(CONF_MIN_VALUE, default=0): cv.int_,
            cv.Optional(CONF_MAX_VALUE, default=100): cv.int_,
            cv.Optional(CONF_INVERT_POSITION, default=False): cv.boolean,
            cv.Optional(CONF_INVERT_POSITION_REPORT, default=False): cv.boolean,
            cv.Optional(CONF_RESTORE_MODE, default="RESTORE"): cv.enum(
                RESTORE_MODES, upper=True
            ),
        },
    ).extend(cv.COMPONENT_SCHEMA),
    validate_range,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await cg.register_component(var, config)
    await cover.register_cover(var, config)

    if CONF_CONTROL_DATAPOINT in config:
        cg.add(var.set_control_id(config[CONF_CONTROL_DATAPOINT]))
    if CONF_DIRECTION_DATAPOINT in config:
        cg.add(var.set_direction_id(config[CONF_DIRECTION_DATAPOINT]))
    cg.add(var.set_position_id(config[CONF_POSITION_DATAPOINT]))
    if CONF_POSITION_REPORT_DATAPOINT in config:
        cg.add(var.set_position_report_id(config[CONF_POSITION_REPORT_DATAPOINT]))
    cg.add(var.set_min_value(config[CONF_MIN_VALUE]))
    cg.add(var.set_max_value(config[CONF_MAX_VALUE]))
    cg.add(var.set_invert_position(config[CONF_INVERT_POSITION]))
    cg.add(var.set_invert_position_report(config[CONF_INVERT_POSITION_REPORT]))
    cg.add(var.set_restore_mode(config[CONF_RESTORE_MODE]))
    paren = await cg.get_variable(config[CONF_UYAT_ID])
    cg.add(var.set_uyat_parent(paren))
