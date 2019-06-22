#include "UtilDefs.h"
#include "Buffer.h"
#include "Config.h"
#include "Interpreter.h"

int main(int argc, char* argv[]) {
  // Parsing the configuration
  const hx::Config cfg(argc, argv);
  cfg.print();

  // Creating the buffer holding the data to interpret
  hx::Buffer buffer(cfg.byte_order_);
  if (!cfg.hex_string_.empty()) {
    buffer.initFromHexString(cfg.hex_string_);
  } else if (!cfg.filepath_.empty()) {
    buffer.initFromFile(cfg.filepath_, cfg.offset_);
  } else {
    _DEATH("Invalid config");
  }

  // Interpreting the data
  hx::Interpreter interpreter(&buffer, &cfg.interpretations_);
  interpreter.performInterpretation();

  return 0;
}
