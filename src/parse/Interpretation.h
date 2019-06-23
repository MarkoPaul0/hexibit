#pragma once

#include <string>

namespace hx {

struct Interpretation {
  enum Type {
    UINT8,
    UINT16,
    UINT32,
    UINT64,
    INT8,
    INT16,
    INT32,
    INT64,
    DOUBLE,
    BOOL,
    IPV4,
    STRING, //null terminated
    CHAR_ARRAY, //must be provided with a length
    SKIPPED // used to skip data between 2 interpretations

    // Any other interesting types?
  };

  Type    type_;
  size_t  size_;

  // Constructor
  Interpretation();

  static const char* interpretationToCstr(Interpretation itp);

  static bool strToInterpretation(const std::string& interpretation_str, Interpretation* interpretation_out);
};

} // namespace hx
