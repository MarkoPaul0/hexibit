#include "Buffer.h"

#include "UtilDefs.h"

namespace hx {

static uint64_t ntoh64(const uint64_t& input) {
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


static int parseByteHalf(const char* p) {
  if (*p >= 'A') { // ABCDEF
    return static_cast<int>(*p - 'A');
  } else { // 0123456789
    return static_cast<int>(*p - '0');
  }
}


Buffer::Buffer() : data_(nullptr), len_(0) {
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
    const int high_half = parseByteHalf(p_in++);
    const int low_half = parseByteHalf(p_in++);
    *p_out = static_cast<char>(high_half*16 + low_half);
    ++p_out;
  }
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
  if (network_byte_order_)
    value = ntoh64(value);

  return value;
}


uint32_t Buffer::getUInt32() const {
  uint32_t value = *reinterpret_cast<const uint32_t*>(data_ + offset_);
  if (network_byte_order_)
    value = htonl(value);

  return value;
}

} // namespace ct
