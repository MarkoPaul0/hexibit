#pragma once

#include <vector>

#include "parse/ByteOrder.h" //TODO: move to the parse directory?
#include "data/IDataReader.h"
#include "print/IConsolePrinter.h"
#include "parse/Interpretation.h"

namespace hx {

class Interpreter {
public:
  // Constructor
  Interpreter(IDataReader* IDataReader, const std::vector<Interpretation>* interpretations, ByteOrder::Enum target_byte_order);

  void performInterpretation(IConsolePrinter* printer);

private:
  IDataReader* const                          data_reader_;
  const std::vector<Interpretation>* const    interpretations_;
  const bool                                  swap_byte_order_;
};

} // namespace hx
