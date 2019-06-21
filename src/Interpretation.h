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
  IP,
  STRING,
  SKIPPED // used to skip data between 2 interpretations

  // Any other interesting types?
};

} // namespace hx
