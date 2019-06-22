#include "ByteOrder.h"

#include <string>

namespace hx {

bool ByteOrder::cstrToByteOrder(const char* byte_order_str, Enum* byte_order_out) {
  const std::string bo(byte_order_str);

  bool valid = true;
  if (bo == "LE" || bo == "LITTLE_ENDIAN")
    *byte_order_out = Enum::LE;
  else if (bo == "BE" || bo == "BIG_ENDIAN")
    *byte_order_out = Enum::BE;
  else
    valid = false;

  return valid;
}


const char* ByteOrder::byteOrderToCstr(Enum bo) {
  switch(bo) {
    case Enum::LE: return "LITTLE_ENDIAN";
    case Enum::BE: return "BIG_ENDIAN";
    default:               _DEATH("Unknown byte order"); //Add %d
  }
}

} // namespace hx
