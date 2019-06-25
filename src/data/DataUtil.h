#pragma once

#include <cstdint>

#include "data/ByteOrder.h"

namespace hx {

struct DataUtil {
  static uint64_t swapByteOrder64(uint64_t input);
  static bool isByteOrderSwappingNeeded(ByteOrder::Enum target_order);
};

} // namespace hx
