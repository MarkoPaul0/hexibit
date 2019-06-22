#pragma once

#include <vector>

#include "Buffer.h"
#include "Interpretation.h"

namespace hx {

class Interpreter {
public:

  // Constructor
  Interpreter(Buffer* buffer, const std::vector<Interpretation>* interpretations);

  void performInterpretation();

private:
  Buffer* const                         buffer_;
  const std::vector<Interpretation>* const    interpretations_;

};


} // namespace hx
