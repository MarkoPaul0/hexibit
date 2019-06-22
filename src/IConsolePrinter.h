#pragma once

#include <string>

#include "Interpretation.h"

namespace hx {

class IConsolePrinter {
public:
  virtual void printHeader() = 0;

  virtual void printFooter() = 0;

  virtual void printInterpretation(const std::string& hex_data, const Interpretation& interpretation, const std::string& interpreted_value_str) = 0;

};

} // namespace hx
