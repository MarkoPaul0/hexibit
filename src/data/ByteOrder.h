#pragma once

namespace hx {

struct ByteOrder {
  enum Enum {
    BE, // Big endian (network byte order)
    LE  // Little endian
  };

  static bool cstrToByteOrder(const char* byte_order_str, Enum* byte_order_out);

  static const char* byteOrderToCstr(Enum bo);

};

} //namespace hx
