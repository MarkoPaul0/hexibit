#include "print/SidePrinter.h"

namespace hx {


void SidePrinter::printHeader() {
}


void SidePrinter::printFooter() {

}


void SidePrinter::printInterpretation(const std::string& hex_data_str, const Interpretation& interpretation, const std::string& interpreted_value_str) {
  printf(PRINT_FORMAT, hex_data_str.c_str(), Interpretation::interpretationToCstr(interpretation), interpreted_value_str.c_str());
  if (hex_data_str.size() > 16) { //TODO: check if it is a string instead
    _DEATH("Not done yet");
    /*
    size_t consumed = 0;
    while (consumed < hex_data_str.size()) {
      size_t len_to_append = std::min(16 - current_data_line_.size(), hex_data_str.size() - consumed);
      current_line_ += hex_data_str.substr(consumed, len_to_append);
      consumed += len_to_append;
      printf("%s\n", current_data_line_.c_str());
    }
    if (!current_line_.empty()) {
      printf("%s\n", current_line_.c_str());
    }
    //TODO: print lines of 16 until hex_data_str is exhausted
    */
  } else {
    // Anything under 8 bytes
    // Can be appended to the current line
  }
  if (current_line_byte_count_ > 16) {
    printf("%s\n", current_line_.c_str());
  }
}

} // namespace hx
