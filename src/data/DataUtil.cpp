#include "data/DataUtil.h"

namespace hx {

//TODO: refactor
uint64_t DataUtil::swapByteOrder64(uint64_t input) {
  uint64_t swapped;
  uint8_t* data = reinterpret_cast<uint8_t*>(&swapped);

  data[0] = static_cast<uint8_t>(input >> 56);
  data[1] = static_cast<uint8_t>(input >> 48);
  data[2] = static_cast<uint8_t>(input >> 40);
  data[3] = static_cast<uint8_t>(input >> 32);
  data[4] = static_cast<uint8_t>(input >> 24);
  data[5] = static_cast<uint8_t>(input >> 16);
  data[6] = static_cast<uint8_t>(input >> 8);
  data[7] = static_cast<uint8_t>(input >> 0);

  return swapped;
}


bool DataUtil::isByteOrderSwappingNeeded(ByteOrder::Enum target_order) {
  union {
    uint16_t  value_;   // 16 bit unsigned integer
    char      mem_[2];  // underlying memory
  } uint16;

  uint16.value_ = 0x0102;
  const bool is_host_big_endian = (uint16.mem_[0] == 0x01);
  const bool is_target_order_big_endian = (target_order == ByteOrder::Enum::BE);

  return (is_host_big_endian != is_target_order_big_endian);
}

} //namespace hx
