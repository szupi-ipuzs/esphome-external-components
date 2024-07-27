import esphome.codegen as cg
from esphome.components import time
import esphome.config_validation as cv
from esphome import pins, automation
from esphome.const import (
    CONF_HOUR,
    CONF_ID,
    CONF_MINUTE,
    CONF_PIN,
    CONF_RUN_DURATION,
    CONF_SECOND,
    CONF_SLEEP_DURATION,
    CONF_TIME_ID,
)

from esphome.core import CORE

deep_sleep_libretiny_ns = cg.esphome_ns.namespace("deep_sleep_libretiny")
DeepSleepLibretiny = deep_sleep_libretiny_ns.class_("DeepSleepLibretiny", cg.Component)
EnterDeepSleepAction = deep_sleep_libretiny_ns.class_(
    "EnterDeepSleepAction", automation.Action
)
PreventDeepSleepAction = deep_sleep_libretiny_ns.class_(
    "PreventDeepSleepAction",
    automation.Action,
    cg.Parented.template(DeepSleepLibretiny),
)
AllowDeepSleepAction = deep_sleep_libretiny_ns.class_(
    "AllowDeepSleepAction",
    automation.Action,
    cg.Parented.template(DeepSleepLibretiny),
)

WakeupPinMode = deep_sleep_libretiny_ns.enum("WakeupPinMode")
WAKEUP_PIN_MODES = {
    "IGNORE": WakeupPinMode.WAKEUP_PIN_MODE_IGNORE,
    "KEEP_AWAKE": WakeupPinMode.WAKEUP_PIN_MODE_KEEP_AWAKE,
    "INVERT_WAKEUP": WakeupPinMode.WAKEUP_PIN_MODE_INVERT_WAKEUP,
}

WakeupCauseToRunDuration = deep_sleep_libretiny_ns.struct("WakeupCauseToRunDuration")

CONF_WAKEUP_PIN_MODE = "wakeup_pin_mode"
CONF_DEFAULT = "default"
CONF_GPIO_WAKEUP_REASON = "gpio_wakeup_reason"
CONF_UNTIL = "until"
CONF_WAKEUP_PINS = "wakeup_pins"

WakeUpPinItem = deep_sleep_libretiny_ns.struct("WakeUpPinItem")
WAKEUP_PINS_SCHEMA = cv.ensure_list(
    cv.Schema(
        {
            cv.Required(CONF_PIN): pins.internal_gpio_input_pin_schema,
            cv.Optional(CONF_WAKEUP_PIN_MODE): cv.All(
                cv.enum(WAKEUP_PIN_MODES), upper=True
            ),
        }
    ),
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(DeepSleepLibretiny),
        cv.Optional(CONF_RUN_DURATION): cv.Any(
            cv.positive_time_period_milliseconds,
        ),
        cv.Optional(CONF_SLEEP_DURATION): cv.positive_time_period_milliseconds,
        cv.Optional(CONF_WAKEUP_PINS): cv.All(
            cv.only_on(["libretiny", "bk72xx"]),
            WAKEUP_PINS_SCHEMA,
        ),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    if CONF_SLEEP_DURATION in config:
        cg.add(var.set_sleep_duration(config[CONF_SLEEP_DURATION]))
    if CONF_RUN_DURATION in config:
        run_duration_config = config[CONF_RUN_DURATION]
        if not isinstance(run_duration_config, dict):
            cg.add(var.set_run_duration(config[CONF_RUN_DURATION]))
        else:
            default_run_duration = run_duration_config[CONF_DEFAULT]
            wakeup_cause_to_run_duration = cg.StructInitializer(
                WakeupCauseToRunDuration,
                ("default_cause", default_run_duration),
                (
                    "gpio_cause",
                    run_duration_config.get(
                        CONF_GPIO_WAKEUP_REASON, default_run_duration
                    ),
                ),
            )
            cg.add(var.set_run_duration(wakeup_cause_to_run_duration))

    if CONF_WAKEUP_PINS in config:
        conf = config[CONF_WAKEUP_PINS]
        for item in conf:
            cg.add(
                var.add_wakeup_pin(
                    await cg.gpio_pin_expression(item[CONF_PIN]),
                    item.get(
                        CONF_WAKEUP_PIN_MODE, WakeupPinMode.WAKEUP_PIN_MODE_IGNORE
                    ),
                )
            )

    # cg.add_define("USE_DEEP_SLEEP")


DEEP_SLEEP_ACTION_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.use_id(DeepSleepLibretiny),
    }
)

DEEP_SLEEP_ENTER_SCHEMA = cv.All(
    automation.maybe_simple_id(
        DEEP_SLEEP_ACTION_SCHEMA.extend(
            cv.Schema(
                {
                    cv.Exclusive(CONF_SLEEP_DURATION, "time"): cv.templatable(
                        cv.positive_time_period_milliseconds
                    ),
                    cv.Optional(CONF_TIME_ID): cv.use_id(time.RealTimeClock),
                }
            )
        )
    ),
    cv.has_none_or_all_keys(CONF_UNTIL, CONF_TIME_ID),
)


@automation.register_action(
    "deep_sleep_libretiny.enter", EnterDeepSleepAction, DEEP_SLEEP_ENTER_SCHEMA
)
async def deep_sleep_enter_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    if CONF_SLEEP_DURATION in config:
        template_ = await cg.templatable(config[CONF_SLEEP_DURATION], args, cg.int32)
        cg.add(var.set_sleep_duration(template_))

    if CONF_UNTIL in config:
        until = config[CONF_UNTIL]
        cg.add(var.set_until(until[CONF_HOUR], until[CONF_MINUTE], until[CONF_SECOND]))

        time_ = await cg.get_variable(config[CONF_TIME_ID])
        cg.add(var.set_time(time_))

    return var


@automation.register_action(
    "deep_sleep_libretiny.prevent",
    PreventDeepSleepAction,
    automation.maybe_simple_id(DEEP_SLEEP_ACTION_SCHEMA),
)
@automation.register_action(
    "deep_sleep_libretiny.allow",
    AllowDeepSleepAction,
    automation.maybe_simple_id(DEEP_SLEEP_ACTION_SCHEMA),
)
async def deep_sleep_action_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    return var
