#pragma once

#include "UtilDefs.h"

namespace hx {

struct ByteOrder {
  enum Enum {
    BE,
    LE
  };

  static bool cstrToByteOrder(const char* byte_order_str, Enum* byte_order_out);

  static const char* byteOrderToCstr(Enum bo);

};

} //namespace hx
