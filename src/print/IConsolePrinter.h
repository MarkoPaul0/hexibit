#pragma once

#include <string>

#include "MainUtils.h"
#include "parse/Interpretation.h"

namespace hx {

class IConsolePrinter {
public:
  virtual ~IConsolePrinter() {
    // Empty virtual destructor
  }

  virtual void printHeader() = 0;

  virtual void printFooter() = 0;

  virtual void printInterpretation(const std::string& hex_data, const Interpretation& interpretation, const std::string& interpreted_value_str) = 0;
};

} // namespace hx
