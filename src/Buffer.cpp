#include "Buffer.h"

#include "UtilDefs.h"

namespace hx {

static uint64_t swapByteOrder64(const uint64_t& input) {
  uint64_t rval;
  uint8_t* data = reinterpret_cast<uint8_t*>(&rval);

  data[0] = static_cast<uint8_t>(input >> 56);
  data[1] = static_cast<uint8_t>(input >> 48);
  data[2] = static_cast<uint8_t>(input >> 40);
  data[3] = static_cast<uint8_t>(input >> 32);
  data[4] = static_cast<uint8_t>(input >> 24);
  data[5] = static_cast<uint8_t>(input >> 16);
  data[6] = static_cast<uint8_t>(input >> 8);
  data[7] = static_cast<uint8_t>(input >> 0);

  return rval;
}



static int parseHexCharacter(char c) {
  int value = 0;
  const char hex_char = static_cast<char>(toupper(c));
  if (hex_char >= 'A')// ABCDEF
    value = static_cast<int>(hex_char - 'A') + 10;
  else // 0123456789
    value = static_cast<int>(hex_char - '0');

  if (value < 0 || value > 15)
    _DEATH("Invalid hexadecimal character '%c'", c);

  return value;
}

static bool isByteOrderSwappingNeeded(ByteOrder::Enum bo) {
  union {
    uint16_t  value_;   // 16 bit unsigned integer
    char      mem_[2];  // underlying memory
  } uint16;

  uint16.value_ = 0x0102;
  const bool is_host_big_endian = (uint16.mem_[0] == 0x01);
  const bool is_desired_order_big_endian = (bo == ByteOrder::Enum::BE);

  return (is_host_big_endian != is_desired_order_big_endian);
}


Buffer::Buffer(ByteOrder::Enum bo) : byte_order_(bo), swap_byte_order_(isByteOrderSwappingNeeded(bo)), data_(nullptr), len_(0) {
}


Buffer::~Buffer() {
  if (data_)
    delete data_;
}


void Buffer::initFromHexString(const std::string& hex_str) {
  const char* p_in = hex_str.c_str();
  len_ = hex_str.size()/2;
  data_ = new char[len_];
  char* p_out = data_;
  for (size_t i = 0; i < len_; ++i) {
    const int high_half = parseHexCharacter(*p_in);
    const int low_half = parseHexCharacter(*(p_in + 1));
    p_in += 2;
    *p_out = static_cast<char>(high_half*16 + low_half);
    ++p_out;
  }
}


void Buffer::initFromFile(const std::string& filepath, size_t offset) {
  _DEATH("initFromFile() is not implemented yet");
}


void Buffer::advanceReadPtr(size_t advance_size) {
//  ASSERT(offset_ + advance_size <= len_);
  offset_ += advance_size;
}


size_t Buffer::getRemainingLength() const {
//  ASSERT(len_ >= offset_);
  return len_ - offset_;
}


uint64_t Buffer::getUInt64() const {
  uint64_t value = *reinterpret_cast<const uint64_t*>(data_ + offset_);
  if (swap_byte_order_)
    value = swapByteOrder64(value);

  return value;
}


uint32_t Buffer::getUInt32() const {
  uint32_t value = *reinterpret_cast<const uint32_t*>(data_ + offset_);
  if (swap_byte_order_)
    value = htonl(value);

  return value;
}


uint16_t Buffer::getUInt16() const {
  uint16_t value = *reinterpret_cast<const uint16_t*>(data_ + offset_);
  if (swap_byte_order_)
    value = htons(value);

  return value;
}


uint8_t Buffer::getUInt8() const {
  return *reinterpret_cast<const uint8_t*>(data_ + offset_);
}


int64_t Buffer::getInt64() const {
  uint64_t unsigned_value = getUInt64();
  return reinterpret_cast<int64_t&>(unsigned_value);
}


int32_t Buffer::getInt32() const {
  uint32_t unsigned_value = getUInt32();
  return reinterpret_cast<int32_t&>(unsigned_value);
}


int16_t Buffer::getInt16() const {
  uint16_t unsigned_value = getUInt16();
  return reinterpret_cast<int16_t&>(unsigned_value);
}


int8_t Buffer::getInt8() const {
  return *reinterpret_cast<const int8_t*>(data_ + offset_);
}


double Buffer::getDouble() const {
  uint64_t unsigned_value = getUInt64();
  return reinterpret_cast<double&>(unsigned_value);
}


bool Buffer::getBool() const {
  return *reinterpret_cast<const bool*>(data_ + offset_);
}

} // namespace ct
