#include "catch.hpp"
#include "parse/Interpretation.h"

TEST_CASE("Interpretation parsing", "[Interpretation]" ){

  SECTION("Parsing uint8") {
    hx::Interpretation itp;
    REQUIRE(hx::Interpretation::strToInterpretation("UINT8", &itp) == true);
    REQUIRE(itp.type_ == hx::Interpretation::UINT8);
    REQUIRE(itp.size_ == 1);
  }

  SECTION("Parsing uint16") {
    hx::Interpretation itp;
    REQUIRE(hx::Interpretation::strToInterpretation("UINT16", &itp) == true);
    REQUIRE(itp.type_ == hx::Interpretation::UINT16);
    REQUIRE(itp.size_ == 2);
  }

  SECTION("Parsing uint32") {
    hx::Interpretation itp;
    REQUIRE(hx::Interpretation::strToInterpretation("UINT32", &itp) == true);
    REQUIRE(itp.type_ == hx::Interpretation::UINT32);
    REQUIRE(itp.size_ == 4);
  }

  SECTION("Parsing uint64") {
    hx::Interpretation itp;
    REQUIRE(hx::Interpretation::strToInterpretation("UINT64", &itp) == true);
    REQUIRE(itp.type_ == hx::Interpretation::UINT64);
    REQUIRE(itp.size_ == 8);
  }

  SECTION("Parsing int8") {
    hx::Interpretation itp;
    REQUIRE(hx::Interpretation::strToInterpretation("INT8", &itp) == true);
    REQUIRE(itp.type_ == hx::Interpretation::INT8);
    REQUIRE(itp.size_ == 1);
  }

  SECTION("Parsing int16") {
    hx::Interpretation itp;
    REQUIRE(hx::Interpretation::strToInterpretation("INT16", &itp) == true);
    REQUIRE(itp.type_ == hx::Interpretation::INT16);
    REQUIRE(itp.size_ == 2);
  }

  SECTION("Parsing int32") {
    hx::Interpretation itp;
    REQUIRE(hx::Interpretation::strToInterpretation("INT32", &itp) == true);
    REQUIRE(itp.type_ == hx::Interpretation::INT32);
    REQUIRE(itp.size_ == 4);
  }

  SECTION("Parsing int64") {
    hx::Interpretation itp;
    REQUIRE(hx::Interpretation::strToInterpretation("INT64", &itp) == true);
    REQUIRE(itp.type_ == hx::Interpretation::INT64);
    REQUIRE(itp.size_ == 8);
  }

  SECTION("Parsing double") {
    hx::Interpretation itp;
    REQUIRE(hx::Interpretation::strToInterpretation("DOUBLE", &itp) == true);
    REQUIRE(itp.type_ == hx::Interpretation::DOUBLE);
    REQUIRE(itp.size_ == 8);
  }


  SECTION("Parsing bool") {
    hx::Interpretation itp;
    REQUIRE(hx::Interpretation::strToInterpretation("BOOL", &itp) == true);
    REQUIRE(itp.type_ == hx::Interpretation::BOOL);
    REQUIRE(itp.size_ == 1);
  }

  SECTION("Parsing ipv4") {
    hx::Interpretation itp;
    REQUIRE(hx::Interpretation::strToInterpretation("IPV4", &itp) == true);
    REQUIRE(itp.type_ == hx::Interpretation::IPV4);
    REQUIRE(itp.size_ == 4);
  }

  SECTION("Parsing null terminated char_array") {
    hx::Interpretation itp;
    REQUIRE(hx::Interpretation::strToInterpretation("CHAR_ARRAY", &itp) == true);
    REQUIRE(itp.type_ == hx::Interpretation::CHAR_ARRAY);
    REQUIRE(itp.size_ == 0);
  }

  SECTION("Parsing lengthed char_array") {
    hx::Interpretation itp;
    REQUIRE(hx::Interpretation::strToInterpretation("CHAR_ARRAY_42", &itp) == true);
    REQUIRE(itp.type_ == hx::Interpretation::CHAR_ARRAY);
    REQUIRE(itp.size_ == 42);
  }

  SECTION("Parsing skipped") {
    hx::Interpretation itp;
    REQUIRE(hx::Interpretation::strToInterpretation("SKIPPED_52", &itp) == true);
    REQUIRE(itp.type_ == hx::Interpretation::SKIPPED);
    REQUIRE(itp.size_ == 52);
  }
}


TEST_CASE("Interpretation to String", "[Interpretation]") {
  hx::Interpretation itp;
  itp.type_ = hx::Interpretation::UINT8;
  REQUIRE(std::string(hx::Interpretation::interpretationToCstr(itp)) == "UINT8");

  itp.type_ = hx::Interpretation::UINT16;
  REQUIRE(std::string(hx::Interpretation::interpretationToCstr(itp)) == "UINT16");

  itp.type_ = hx::Interpretation::UINT32;
  REQUIRE(std::string(hx::Interpretation::interpretationToCstr(itp)) == "UINT32");

  itp.type_ = hx::Interpretation::UINT64;
  REQUIRE(std::string(hx::Interpretation::interpretationToCstr(itp)) == "UINT64");

  itp.type_ = hx::Interpretation::INT8;
  REQUIRE(std::string(hx::Interpretation::interpretationToCstr(itp)) == "INT8");

  itp.type_ = hx::Interpretation::INT16;
  REQUIRE(std::string(hx::Interpretation::interpretationToCstr(itp)) == "INT16");

  itp.type_ = hx::Interpretation::INT32;
  REQUIRE(std::string(hx::Interpretation::interpretationToCstr(itp)) == "INT32");

  itp.type_ = hx::Interpretation::INT64;
  REQUIRE(std::string(hx::Interpretation::interpretationToCstr(itp)) == "INT64");

  itp.type_ = hx::Interpretation::DOUBLE;
  REQUIRE(std::string(hx::Interpretation::interpretationToCstr(itp)) == "DOUBLE");

  itp.type_ = hx::Interpretation::BOOL;
  REQUIRE(std::string(hx::Interpretation::interpretationToCstr(itp)) == "BOOL");

  itp.type_ = hx::Interpretation::IPV4;
  REQUIRE(std::string(hx::Interpretation::interpretationToCstr(itp)) == "IPV4");

  itp.type_ = hx::Interpretation::CHAR_ARRAY;
  REQUIRE(std::string(hx::Interpretation::interpretationToCstr(itp)) == "CHAR_ARRAY");

  itp.type_ = hx::Interpretation::SKIPPED;
  REQUIRE(std::string(hx::Interpretation::interpretationToCstr(itp)) == "SKIPPED");
}

