#pragma once

#include <string>

#include "data/ByteOrder.h"
#include "data/IDataReader.h"

namespace hx {

class Buffer : public IDataReader {
public:
  // Constructor
  Buffer(const std::string& hex_str, ByteOrder::Enum byte_order);

  // Destructor
  ~Buffer();

  void advanceReadPtr(size_t advance_size) override;
  size_t getRemainingLength() const override;

  const char* getData() const override;
  // Remove the need for copy
  std::string getDataAsHexString(size_t num_bytes) const override;

  uint64_t    getUInt64()     const override;
  uint32_t    getUInt32()     const override;
  uint16_t    getUInt16()     const override;
  uint8_t     getUInt8()      const override;

  int64_t     getInt64()      const override;
  int32_t     getInt32()      const override;
  int16_t     getInt16()      const override;
  int8_t      getInt8()       const override;

  double      getDouble()     const override;
  bool        getBool()       const override;

private:
  const bool              swap_byte_order_;
  char*                   data_;
  size_t                  len_;
  size_t                  offset_;
};

} // namespace hx
