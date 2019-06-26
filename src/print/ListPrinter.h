#pragma once

#include <string>

#include "parse/Interpretation.h"
#include "print/IConsolePrinter.h"

namespace hx {

/*
  Implementation of IConsolePrinter, printing the interpretation data in the form of a list.
*/
class ListPrinter : public IConsolePrinter {
public:
  void startPrint() override;
  void endPrint() override;
  void printInterpretation(const std::string& hex_data, const Interpretation& interpretation, const std::string& interpreted_value_str) override;
};

} // namespace hx
