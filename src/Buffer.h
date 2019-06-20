#pragma once

#include <string>

namespace hx {

class Buffer {
public:
  Buffer();

  ~Buffer();

  void initFromHexString(const std::string& hex_str);

  void advanceReadPtr(size_t advance_size);

  size_t getRemainingLength() const;

  uint64_t getUInt64() const;
  uint32_t getUInt32() const;

private:
  char*       data_;
  size_t      len_;
  size_t      offset_ = 0;
  bool        network_byte_order_ = false;
};

} // namespace hx
