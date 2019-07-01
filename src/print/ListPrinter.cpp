#include "print/ListPrinter.h"

namespace hx {

static constexpr char PRINT_FORMAT[18] = "%-32s  %-20s  %s\n";


void ListPrinter::startPrint() {
  printf(PRINT_FORMAT, "Data", "Interpretation", "Value");
  printf("-----------------------------------------------------------------------\n");
}


void ListPrinter::endPrint() {
}


void ListPrinter::printInterpretation(const std::string& hex_data_str, const Interpretation& interpretation, const std::string& interpreted_value_str) {
  const size_t len = hex_data_str.size();
  const std::string hex_copy = (len > 32) ? (hex_data_str.substr(0, 30) + "..") : hex_data_str;
  printf(PRINT_FORMAT, hex_copy.c_str(), Interpretation::interpretationToCstr(interpretation), interpreted_value_str.c_str());
}

} // namespace hx
