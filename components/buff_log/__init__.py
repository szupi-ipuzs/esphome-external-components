import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID
)

DEPENDENCIES = ["logger"]

CODEOWNERS = ["@szupi-ipuzs"]
buff_log_ns = cg.esphome_ns.namespace("buff_log")

CONF_SIZE_LIMIT = "size_limit"
CONF_DUMP_LINES_PER_LOOP = "dump_lines_per_loop"
CONF_ON_DUMP_LINE = "on_dump_line"

BufferingLogger = buff_log_ns.class_('BufferingLogger', cg.Component)

CONFIG_SCHEMA = cv.All(
   cv.Schema({
      cv.GenerateID(CONF_ID): cv.declare_id(BufferingLogger),
      cv.Required(CONF_SIZE_LIMIT): cv.int_range(min=1024, max=1048576),
      cv.Optional(CONF_DUMP_LINES_PER_LOOP, default=20): cv.int_range(min=5, max=100),
      cv.Required(CONF_ON_DUMP_LINE): cv.lambda_
   })
)

async def to_code(config):
   lambda_ = await cg.process_lambda(
         config[CONF_ON_DUMP_LINE],
         [(buff_log_ns.LogItem, "item")],
         return_type=cg.void,
   )
   var = cg.new_Pvariable(config[CONF_ID], config[CONF_SIZE_LIMIT], config[CONF_DUMP_LINES_PER_LOOP], lambda_)
   await cg.register_component(var, config)
