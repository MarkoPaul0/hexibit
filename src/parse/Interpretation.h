#pragma once

#include <string>

namespace hx {

struct Interpretation {
  enum Type {
    UINT8       = 0,
    UINT16      = 1,
    UINT32      = 2,
    UINT64      = 3,
    INT8        = 4,
    INT16       = 5,
    INT32       = 6,
    INT64       = 7,
    DOUBLE      = 8,
    BOOL        = 9,
    IPV4        = 10,
    CHAR_ARRAY  = 11, // Can be provided with a length, e.g. "CHAR_ARRAY_3" for 3 characters, without length it is treated as a null terminated char array
    SKIPPED     = 12  // Used to skip data between 2 interpretations. Must be provided with a length

    // TODO: Any other interesting types?
  };

  Type    type_;  // Interpretation type
  size_t  size_;  // Size of the interpretation. This size may be static, for instance UINT32 is 4 bytes,
                  // or dynamic, for instance CHAR_ARRAY.

  // Constructor
  Interpretation();

  // Convert a string into an Interpretation. Return true if the conversion was a success, false otherwise.
  static bool strToInterpretation(const std::string& interpretation_str, Interpretation* interpretation_out);

  // Convert an interpretation into a c string
  static const char* interpretationToCstr(Interpretation itp);
};

} // namespace hx
