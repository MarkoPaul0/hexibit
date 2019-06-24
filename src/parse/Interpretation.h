#pragma once

#include <string>

namespace hx {

struct Interpretation {
  enum Type {
    UINT8 = 0,
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
    //TODO: merge string and char_array
    STRING,     // Null terminated
    CHAR_ARRAY, // Must be provided with a length, e.g. "CHAR_ARRAY_3" for 3 characters
    SKIPPED     // Used to skip data between 2 interpretations. Must be provided with a length

    // TODO: Any other interesting types?
  };

  Type    type_;
  size_t  size_;

  // Constructor
  Interpretation();

  static const char* interpretationToCstr(Interpretation itp);

  static bool strToInterpretation(const std::string& interpretation_str, Interpretation* interpretation_out);
};

} // namespace hx
