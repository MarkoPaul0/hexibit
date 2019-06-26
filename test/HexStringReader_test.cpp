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
}

/*
TEST_CASE("HexStringReader advanceWritePtr()", "[HexStringReader]") {
    hx::HexStringReader bb;
    const char* const write_ptr = bb.getWritePtr(0);

    SECTION("Normal case") {
        bb.advanceWritePtr(10);
        bb.advanceReadPtr(7);
        REQUIRE(bb.readableSize() == 3);
    }

    SECTION("Edge case 0") {
        bb.advanceWritePtr(10);
        bb.advanceReadPtr(0);
        REQUIRE(bb.readableSize() == 10);
    }

    SECTION("Edge case 1") {
        bb.advanceWritePtr(10);
        bb.advanceReadPtr(1);
        REQUIRE(bb.readableSize() == 9);
    }


    SECTION("Edge case write ptr - 1") {
        bb.advanceWritePtr(10);
        bb.advanceReadPtr(9);
        REQUIRE(bb.readableSize() == 1);
    }

    SECTION("Edge case write ptr") {
        bb.advanceWritePtr(10);
        bb.advanceReadPtr(10);
        REQUIRE(bb.readableSize() == 0);
    }

    SECTION("Edge case, past write ptr") {
        bb.advanceWritePtr(10);
        REQUIRE_THROWS(bb.advanceReadPtr(11));
    }


    SECTION("Proper reset when read pointer advanced to write ptr") {
        bb.advanceWritePtr(10);
        REQUIRE(bb.writableSize() == hx::HexStringReader::MAX_SIZE - 10);
        bb.advanceReadPtr(10);
        REQUIRE(bb.readableSize() == 0);
        REQUIRE(bb.writableSize() == hx::HexStringReader::MAX_SIZE);
        REQUIRE(bb.getWritePtr(1) == write_ptr);
    }
}

TEST_CASE("HexStringReader getWritePtr", "[HexStringReader]") {
    hx::HexStringReader bb;
    const char* start_ptr = bb.getReadPtr();;
    bb.advanceWritePtr(10);

    SECTION("Normal case") {
        REQUIRE(start_ptr + 10 == bb.getWritePtr(0));
        REQUIRE(start_ptr + 10 == bb.getWritePtr(1));
        REQUIRE(start_ptr + 10 == bb.getWritePtr(9));
        REQUIRE(bb.getWritePtr(0) == bb.getWritePtr(hx::HexStringReader::MAX_SIZE - 10));
    }

    SECTION("Needing space than available") {
        REQUIRE_THROWS(bb.getWritePtr(hx::HexStringReader::MAX_SIZE - 9));
    }
}
*/
