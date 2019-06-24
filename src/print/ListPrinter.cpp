#include "print/ListPrinter.h"

namespace hx {

static constexpr char PRINT_FORMAT[16] = "%-32s %-20s %s\n";


void ListPrinter::printHeader() {
  printf(PRINT_FORMAT, "Data", "Interpretation", "Value");
  printf("-----------------------------------------------------------\n");
}


void ListPrinter::printFooter() {

}


void ListPrinter::printInterpretation(const std::string& hex_data_str, const Interpretation& interpretation, const std::string& interpreted_value_str) {
  printf(PRINT_FORMAT, hex_data_str.c_str(), Interpretation::interpretationToCstr(interpretation), interpreted_value_str.c_str());

}

} // namespace hx
