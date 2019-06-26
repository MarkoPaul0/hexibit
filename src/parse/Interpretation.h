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

  Type    type_;  // Interpretation type
  size_t  size_;  // Size of the interpretation. This size may be static, for instance UINT32 is 4 bytes,
                  // or dynamic, for instance CHAR_ARRAY.

  // Constructor
  Interpretation();

  // Convert a string into an Interpretatation. Return true if the conversion was a success, false otherwise.
  static bool strToInterpretation(const std::string& interpretation_str, Interpretation* interpretation_out);

  // Convert an interpretation into a c string
  static const char* interpretationToCstr(Interpretation itp);
};

} // namespace hx
