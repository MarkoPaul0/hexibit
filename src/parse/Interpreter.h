#pragma once

#include <vector>

#include "data/IDataReader.h"
#include "print/IConsolePrinter.h"
#include "parse/Interpretation.h"

namespace hx {

class Interpreter {
public:
  // Constructor
  Interpreter(IDataReader* IDataReader, const std::vector<Interpretation>* interpretations);

  void performInterpretation(IConsolePrinter* printer);

private:
  IDataReader* const                          data_reader_;
  const std::vector<Interpretation>* const    interpretations_;
};

} // namespace hx
