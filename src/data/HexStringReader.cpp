#include "data/HexStringReader.h"

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

static const char* hexStringToData(const std::string& hex_str) {
  const char* p_in = hex_str.c_str();
  size_t len = hex_str.size()/2;
  char* data = new char[len];
  char* p_out = data;
  for (size_t i = 0; i < len; ++i) {
    const int high_half = parseHexCharacter(*p_in);
    const int low_half = parseHexCharacter(*(p_in + 1));
    *p_out = static_cast<char>(high_half*16 + low_half);
    // 2 hex characters = 1 byte of data
    p_in += 2;
    p_out++;
  }

  return data;
}


HexStringReader::HexStringReader(const std::string& hex_str) : data_(hexStringToData(hex_str)), len_(hex_str.size()/2), offset_(0) {
}


HexStringReader::~HexStringReader() {
  delete [] data_;
}


void HexStringReader::advanceReadPtr(size_t num_bytes) {
  _ASSERT(offset_ + num_bytes <= len_);
  offset_ += num_bytes;
}


size_t HexStringReader::getRemainingLength() const {
  _ASSERT(len_ >= offset_);
  return len_ - offset_;
}


const char* HexStringReader::getReadPtr(size_t num_bytes) {
  if (num_bytes > getRemainingLength())
    _DEATH("Trying to read bytes beyond data size!");

  return data_ + offset_;
}

} // namespace ct
