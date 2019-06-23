#include "MainUtils.h"

#include "config/Config.h"
#include "data/Buffer.h"
#include "parse/Interpreter.h"
#include "print/ListPrinter.h"

int main(int argc, char* argv[]) {
  // Parsing the configuration
  const hx::Config cfg(argc, argv);
  cfg.print();
  //TODO: validate and print usage if invalid

  // Creating the buffer holding the data to interpret
  hx::Buffer buffer(cfg.byte_order_);
  if (!cfg.hex_string_.empty()) {
    buffer.initFromHexString(cfg.hex_string_);
  } else if (!cfg.filepath_.empty()) {
    buffer.initFromFile(cfg.filepath_, cfg.offset_);
  } else {
    _DEATH("Invalid config");
  }

  // Creating the interpreter and the printer
  hx::Interpreter interpreter(&buffer, &cfg.interpretations_);
  hx::IConsolePrinter* printer = new hx::ListPrinter();

  // Interpreting the data
  interpreter.performInterpretation(printer);

  delete printer;
  return 0;
}
