#include "catch.hpp"

#include <cstring>

#include "data/FileReader.h"
#include "MainUtils.h"


static int parseHexCharacter(char c) {
  int value = 0;
  const char hex_char = static_cast<char>(toupper(c));
  if (hex_char >= 'A') // ABCDEF
    value = static_cast<int>(hex_char - 'A') + 10;
  else // 0123456789
    value = static_cast<int>(hex_char - '0');

  if (value < 0 || value > 15)
    _DEATH("Invalid hexadecimal character '%c'", c);

  return value;
}


static const char* hexDataToBinaryData(const std::string& hex_data, size_t len) {
  const char* p_in = hex_data.c_str();
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


static void createBinaryFile(const std::string& filepath, const std::string& hex_data) {
  FILE* file = fopen(filepath.c_str(), "w");
  if (!file)
    _DEATH("Could not create file at '%s'! (errno: %d)", filepath.c_str(), errno);

  const size_t data_len = hex_data.size() / 2;
  const char* data = hexDataToBinaryData(hex_data, data_len);

  if (fwrite(data, sizeof(char), data_len, file) != data_len)
    _DEATH("Could not write data to file '%s'!", filepath.c_str());

  if (fflush(file) != 0)
    _DEATH("fflush() failed on file '%s'", filepath.c_str());

  if (fclose(file) != 0)
    _DEATH("fclose() failed on file '%s'", filepath.c_str());

  delete [] data;
}


TEST_CASE("FileReader properly initialized", "[FileReader]") {
  const std::string filepath = "/tmp/file_reader_ut1";
  createBinaryFile(filepath, "AB0102CD");
  hx::FileReader fr(filepath, /*offset*/0);
  REQUIRE(fr.getRemainingLength() == 4);
  const uint32_t expected_data = 0xCD0201AB;
  REQUIRE(*reinterpret_cast<const uint32_t*>(fr.getReadPtr(4)) == expected_data);
}


TEST_CASE("FileReader advanceReadPtr()", "[FileReader]") {
  const std::string filepath = "/tmp/file_reader_ut2";
  createBinaryFile(filepath, "0102030405060708090A0B0C0D0E0F");
  hx::FileReader fr(filepath, /*offset*/0);
  const char* const read_ptr = fr.getReadPtr(15);
  char orig_data[15];
  memcpy(orig_data, read_ptr, 15);
  REQUIRE(fr.getRemainingLength() == 15);

  SECTION("Normal case") {
    fr.advanceReadPtr(10);
    REQUIRE(fr.getRemainingLength() == 5);
    REQUIRE(static_cast<uint8_t>(*fr.getReadPtr(1)) == static_cast<uint8_t>(orig_data[10]));
  }

  SECTION("Edge case 0") {
    fr.advanceReadPtr(0);
    REQUIRE(fr.getRemainingLength() == 15);
    REQUIRE(static_cast<uint8_t>(*fr.getReadPtr(1)) == static_cast<uint8_t>(orig_data[0]));
  }

  SECTION("Edge case 1") {
    fr.advanceReadPtr(1);
    REQUIRE(fr.getRemainingLength() == 14);
    REQUIRE(static_cast<uint8_t>(*fr.getReadPtr(1)) == static_cast<uint8_t>(orig_data[1]));
  }

  SECTION("Edge case size - 1") {
    fr.advanceReadPtr(14);
    REQUIRE(fr.getRemainingLength() == 1);
    REQUIRE(static_cast<uint8_t>(*fr.getReadPtr(1)) == static_cast<uint8_t>(orig_data[14]));
  }

  SECTION("Edge case size") {
    fr.advanceReadPtr(15);
    REQUIRE(fr.getRemainingLength() == 0);
  }
}


TEST_CASE("FileReader getReadPtr()", "[FileReader]") {
  const std::string filepath = "/tmp/file_reader_ut3";
  createBinaryFile(filepath, "010203040506 ");
  hx::FileReader fr(filepath, /*offset*/0);
  REQUIRE(fr.getRemainingLength() == 6);

  SECTION("Normal case") {
    const char* read_ptr= fr.getReadPtr(4);
    REQUIRE(read_ptr[0] == 0x01);
    REQUIRE(read_ptr[1] == 0x02);
    REQUIRE(read_ptr[2] == 0x03);
    REQUIRE(read_ptr[3] == 0x04);
  }

  SECTION("Edge case 1") {
    const char* read_ptr= fr.getReadPtr(1);
    REQUIRE(read_ptr[0] == 0x01);
  }

  SECTION("Normal case len - 1") {
    const char* read_ptr= fr.getReadPtr(5);
    REQUIRE(read_ptr[0] == 0x01);
    REQUIRE(read_ptr[1] == 0x02);
    REQUIRE(read_ptr[2] == 0x03);
    REQUIRE(read_ptr[3] == 0x04);
    REQUIRE(read_ptr[4] == 0x05);
  }

  SECTION("Normal case len") {
    const char* read_ptr= fr.getReadPtr(6);
    REQUIRE(read_ptr[0] == 0x01);
    REQUIRE(read_ptr[1] == 0x02);
    REQUIRE(read_ptr[2] == 0x03);
    REQUIRE(read_ptr[3] == 0x04);
    REQUIRE(read_ptr[4] == 0x05);
    REQUIRE(read_ptr[5] == 0x06);
  }

  SECTION("Normal case end") {
    fr.advanceReadPtr(4);
    const char* read_ptr= fr.getReadPtr(2);
    REQUIRE(read_ptr[0] == 0x05);
    REQUIRE(read_ptr[1] == 0x06);
  }
}
