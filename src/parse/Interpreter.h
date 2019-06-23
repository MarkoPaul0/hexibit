#pragma once

#include <vector>

#include "data/Buffer.h"
#include "print/IConsolePrinter.h"
#include "parse/Interpretation.h"

namespace hx {

class Interpreter {
public:
  // Constructor
  Interpreter(Buffer* buffer, const std::vector<Interpretation>* interpretations);

  void performInterpretation(IConsolePrinter* printer);

private:
  Buffer* const                               buffer_;
  const std::vector<Interpretation>* const    interpretations_;
};

} // namespace hx
