#pragma once

namespace hx {

enum class Interpretation {
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

} // namespace hx
