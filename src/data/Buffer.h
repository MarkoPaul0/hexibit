#pragma once

#include <string>

#include "data/ByteOrder.h"

namespace hx {

class Buffer {
public:
  // Constructor
  Buffer(ByteOrder::Enum byte_order);

  // Destructor
  ~Buffer();

  void initFromHexString(const std::string& hex_str);
  void initFromFile(const std::string& filepath, size_t offset);

  void advanceReadPtr(size_t advance_size);
  size_t getRemainingLength() const;

  const char* getData() const;
  // Remove the need for copy
  std::string getDataAsHexString(size_t num_bytes) const;

  uint64_t    getUInt64()     const;
  uint32_t    getUInt32()     const;
  uint16_t    getUInt16()     const;
  uint8_t     getUInt8()      const;

  int64_t     getInt64()      const;
  int32_t     getInt32()      const;
  int16_t     getInt16()      const;
  int8_t      getInt8()       const;

  double      getDouble()     const;
  bool        getBool()       const;
  std::string getIPv4String() const;

private:
  const ByteOrder::Enum   byte_order_;
  const bool              swap_byte_order_;
  char*                   data_;
  size_t                  len_;
  size_t                  offset_;
};

} // namespace hx
