#include "data/HexStringReader.h"

#include "data/DataUtil.h"
#include "MainUtils.h"

namespace hx {

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


HexStringReader::HexStringReader(const std::string& hex_str, ByteOrder::Enum bo) : swap_byte_order_(DataUtil::isByteOrderSwappingNeeded(bo)), data_(nullptr), len_(0), offset_(0) {
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


HexStringReader::~HexStringReader() {
  if (data_)
    delete data_;
}


void HexStringReader::advanceReadPtr(size_t advance_size) {
//  ASSERT(offset_ + advance_size <= len_);
  offset_ += advance_size;
}


size_t HexStringReader::getRemainingLength() const {
//  ASSERT(len_ >= offset_);
  return len_ - offset_;
}


const char* HexStringReader::getData() const {
  return data_ + offset_;
}


//TODO: remove
std::string HexStringReader::getDataAsHexString(size_t num_bytes) const {
  if (num_bytes > getRemainingLength())
    _DEATH("Not enough data available");

  const size_t str_len = 2*num_bytes;
  char* str_buf = new char[str_len];
  for(size_t i = 0; i < num_bytes; ++i)
    sprintf(&str_buf[2*i], "%02X", data_[offset_ + i]);

  std::string str(str_buf, str_len);
  return str;
}


uint64_t HexStringReader::getUInt64() const {
  uint64_t value = *reinterpret_cast<const uint64_t*>(data_ + offset_);
  if (swap_byte_order_)
    value = DataUtil::swapByteOrder64(value);

  return value;
}


uint32_t HexStringReader::getUInt32() const {
  uint32_t value = *reinterpret_cast<const uint32_t*>(data_ + offset_);
  if (swap_byte_order_)
    value = htonl(value);

  return value;
}


uint16_t HexStringReader::getUInt16() const {
  uint16_t value = *reinterpret_cast<const uint16_t*>(data_ + offset_);
  if (swap_byte_order_)
    value = htons(value);

  return value;
}


uint8_t HexStringReader::getUInt8() const {
  return *reinterpret_cast<const uint8_t*>(data_ + offset_);
}


int64_t HexStringReader::getInt64() const {
  uint64_t unsigned_value = getUInt64();
  return reinterpret_cast<int64_t&>(unsigned_value);
}


int32_t HexStringReader::getInt32() const {
  uint32_t unsigned_value = getUInt32();
  return reinterpret_cast<int32_t&>(unsigned_value);
}


int16_t HexStringReader::getInt16() const {
  uint16_t unsigned_value = getUInt16();
  return reinterpret_cast<int16_t&>(unsigned_value);
}


int8_t HexStringReader::getInt8() const {
  return *reinterpret_cast<const int8_t*>(data_ + offset_);
}


double HexStringReader::getDouble() const {
  uint64_t unsigned_value = getUInt64();
  return reinterpret_cast<double&>(unsigned_value);
}


bool HexStringReader::getBool() const {
  return *reinterpret_cast<const bool*>(data_ + offset_);
}

} // namespace ct
