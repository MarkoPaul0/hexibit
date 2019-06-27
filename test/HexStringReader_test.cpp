//#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "data/HexStringReader.h"

TEST_CASE("HexStringReader properly initialized", "[HexStringReader]" ){
    hx::HexStringReader hsr("AB0102CD");
    REQUIRE(hsr.getRemainingLength() == 4);
    const uint32_t expected_data = 0xCD0201AB;
    REQUIRE(*reinterpret_cast<const uint32_t*>(hsr.getReadPtr(4)) == expected_data);
}

TEST_CASE("HexStringReader advanceReadPtr()", "[HexStringReader]") {
    hx::HexStringReader hsr("0102030405060708090A0B0C0D0E0F");
    const char* const read_ptr = hsr.getReadPtr(15);
    REQUIRE(hsr.getRemainingLength() == 15);

    SECTION("Normal case") {
        hsr.advanceReadPtr(10);
        REQUIRE(hsr.getRemainingLength() == 5);
        REQUIRE(hsr.getReadPtr(1) == read_ptr + 10);
    }

    SECTION("Edge case 0") {
        hsr.advanceReadPtr(0);
        REQUIRE(hsr.getRemainingLength() == 15);
        REQUIRE(hsr.getReadPtr(1) == read_ptr);
    }

    SECTION("Edge case 1") {
        hsr.advanceReadPtr(1);
        REQUIRE(hsr.getRemainingLength() == 14);
        REQUIRE(hsr.getReadPtr(1) == read_ptr + 1);
    }

    SECTION("Edge case size - 1") {
        hsr.advanceReadPtr(14);
        REQUIRE(hsr.getRemainingLength() == 1);
        REQUIRE(hsr.getReadPtr(1) == read_ptr + 14);
    }

    SECTION("Edge case size") {
        hsr.advanceReadPtr(15);
        REQUIRE(hsr.getRemainingLength() == 0);
    }
}


TEST_CASE("HexStringReader getReadPtr()", "[HexStringReader]") {
    hx::HexStringReader hsr("010203040506");
    const char* const read_ptr = hsr.getReadPtr(6);
    REQUIRE(hsr.getRemainingLength() == 6);

    SECTION("Normal case") {
        const char* read_ptr= hsr.getReadPtr(4);
        REQUIRE(read_ptr[0] == 0x01);
        REQUIRE(read_ptr[1] == 0x02);
        REQUIRE(read_ptr[2] == 0x03);
        REQUIRE(read_ptr[3] == 0x04);
    }

    SECTION("Edge case 1") {
        const char* read_ptr= hsr.getReadPtr(1);
        REQUIRE(read_ptr[0] == 0x01);
    }

    SECTION("Normal case len - 1") {
        const char* read_ptr= hsr.getReadPtr(5);
        REQUIRE(read_ptr[0] == 0x01);
        REQUIRE(read_ptr[1] == 0x02);
        REQUIRE(read_ptr[2] == 0x03);
        REQUIRE(read_ptr[3] == 0x04);
        REQUIRE(read_ptr[4] == 0x05);
    }

    SECTION("Normal case len") {
        const char* read_ptr= hsr.getReadPtr(6);
        REQUIRE(read_ptr[0] == 0x01);
        REQUIRE(read_ptr[1] == 0x02);
        REQUIRE(read_ptr[2] == 0x03);
        REQUIRE(read_ptr[3] == 0x04);
        REQUIRE(read_ptr[4] == 0x05);
        REQUIRE(read_ptr[5] == 0x06);
    }

    SECTION("Normal case end") {
        hsr.advanceReadPtr(4);
        const char* read_ptr= hsr.getReadPtr(2);
        REQUIRE(read_ptr[0] == 0x05);
        REQUIRE(read_ptr[1] == 0x06);
    }
}
