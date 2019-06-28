#include "catch.hpp"
#include "parse/ByteOrder.h"

TEST_CASE("ByteOrder parsing", "[ByteOrder]" ){

  SECTION("Little Endian") {
    const char* bo_str = "LITTLE_ENDIAN";
    hx::ByteOrder::Enum byte_order;
    REQUIRE(hx::ByteOrder::cstrToByteOrder(bo_str, &byte_order) == true);
    REQUIRE(byte_order == hx::ByteOrder::LE);
  }

  SECTION("LE") {
    const char* bo_str = "LE";
    hx::ByteOrder::Enum byte_order;
    REQUIRE(hx::ByteOrder::cstrToByteOrder(bo_str, &byte_order) == true);
    REQUIRE(byte_order == hx::ByteOrder::LE);
  }

  SECTION("Big Endian") {
    const char* bo_str = "BIG_ENDIAN";
    hx::ByteOrder::Enum byte_order;
    REQUIRE(hx::ByteOrder::cstrToByteOrder(bo_str, &byte_order) == true);
    REQUIRE(byte_order == hx::ByteOrder::BE);
  }

  SECTION("BE") {
    const char* bo_str = "BE";
    hx::ByteOrder::Enum byte_order;
    REQUIRE(hx::ByteOrder::cstrToByteOrder(bo_str, &byte_order) == true);
    REQUIRE(byte_order == hx::ByteOrder::BE);
  }

  SECTION("Anything else") {
    hx::ByteOrder::Enum byte_order;
    REQUIRE(hx::ByteOrder::cstrToByteOrder("liTTlE_eNdIan", &byte_order) == false);
    REQUIRE(hx::ByteOrder::cstrToByteOrder("bIg_EnDiAn", &byte_order) == false);
    REQUIRE(hx::ByteOrder::cstrToByteOrder("blurb", &byte_order) == false);
  }
}
