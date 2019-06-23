#pragma once

#include <string>

#include "data/ByteOrder.h"

namespace hx {

class Buffer {
public:
  Buffer(ByteOrder::Enum byte_order);

  ~Buffer();

  void initFromHexString(const std::string& hex_str);
  void initFromFile(const std::string& filepath, size_t offset);

  void advanceReadPtr(size_t advance_size);

  size_t getRemainingLength() const;

  std::string getDataAsHexString(size_t num_bytes) {
    if (num_bytes > getRemainingLength())
      _DEATH("Not enough data available");

    const size_t str_len = 2*num_bytes;
    char* str_buf = new char[str_len];
    for(size_t i = 0; i < num_bytes; ++i)
      sprintf(&str_buf[2*i], "%02X", data_[offset_ + i]);

    std::string str(str_buf, str_len);
    return str;
  }

  uint64_t getUInt64() const;
  uint32_t getUInt32() const;
  uint16_t getUInt16() const;
  uint8_t getUInt8() const;

  int64_t getInt64() const;
  int32_t getInt32() const;
  int16_t getInt16() const;
  int8_t getInt8() const;

  double getDouble() const;
  bool getBool() const;
  std::string getIPv4String() const;

private:
  const ByteOrder::Enum   byte_order_;
  const bool        swap_byte_order_;
  char*             data_;
  size_t            len_;
  size_t            offset_;
};

} // namespace hx
