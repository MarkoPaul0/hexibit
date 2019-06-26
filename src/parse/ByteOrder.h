#pragma once

namespace hx {

struct ByteOrder {
  enum Enum {
    BE = 0, // Big endian (network byte order)
    LE = 1  // Little endian
  };

  // Convert a byte order string into a ByteOrder::Enum.
  // Return true if the conversion was successful, false otherwise.
  static bool cstrToByteOrder(const char* byte_order_str, Enum* byte_order_out);

  // Converts a ByteOrder::Enum to a c string.
  static const char* byteOrderToCstr(Enum bo);
};

} // namespace hx
