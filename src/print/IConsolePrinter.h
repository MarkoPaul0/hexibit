#pragma once

#include <string>

#include "parse/Interpretation.h"

namespace hx {

/*
  Abstract class defining a simple data printing interface. It is meant to be used
  to abstract away the complexity of printing the data in different ways.
*/
class IConsolePrinter {
public:
  virtual ~IConsolePrinter() {
    // Empty virtual destructor
  }

  // Method to be called before invoking printInterpretation() which allows the printing
  // of heading information.
  virtual void startPrint() = 0;

  // Method to provide information related to a single interpretation of the data.
  // Must be called after the invocation of startPrint() and before the invocation of endPrint().
  // Input arguments:
  //    hex_data:                 chunk of data that is interpreted.
  //    interpretation:           interpretation that was performed on hex_data.
  //    interpreted_value_str:    value of the interpretation of hex_data.
  virtual void printInterpretation(const std::string& hex_data, const Interpretation& interpretation, const std::string& interpreted_value_str) = 0;

  // Method to be called after the last invocation of printInterpretation(). This allows
  // for the printing and flushing of the data.
  // Additionally, final information could be printed as well.
  virtual void endPrint() = 0;
};

} // namespace hx
