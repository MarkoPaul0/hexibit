//#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <queue>

#include "data/HexStringReader.h"
#include "parse/Interpreter.h"

namespace hx {

struct PrintEvent {
  enum Type {
    PE_START,
    PE_PRINT_INTERPRETATION,
    PE_END
  };

  PrintEvent() {
  }

  PrintEvent(Type type) : type_(type) {
  }

  Type            type_;

  Interpretation  interpretation_;    // only valid if type is PE_PRINT_INTERPRETATION
  std::string     hex_data_;          // only valid if type is PE_PRINT_INTERPRETATION
  std::string     interpreted_str_;   // only valid if type is PE_PRINT_INTERPRETATION
};


class PrinterMock : public IConsolePrinter {
public:
  void startPrint() override {
    PrintEvent pe(PrintEvent::Type::PE_START);
    events_.push(pe);
  }

  void printInterpretation(const std::string& hex_data, const Interpretation& interpretation, const std::string& interpreted_value_str) override {
    PrintEvent pe(PrintEvent::Type::PE_PRINT_INTERPRETATION);
    pe.interpretation_ = interpretation;
    pe.hex_data_ = hex_data;
    pe.interpreted_str_ = interpreted_value_str;
    events_.push(pe);
  }

  void endPrint() override {
    PrintEvent pe(PrintEvent::Type::PE_END);
    events_.push(pe);
  }

  std::queue<PrintEvent> events_;
};

}


TEST_CASE("Basic case", "[Interpreter]") {
  hx::HexStringReader hsr("01020304");
  hx::Interpretation i;
  i.type_ = hx::Interpretation::UINT8;
  i.size_ = 1;
  std::vector<hx::Interpretation> itps = { i, i, i, i };

  hx::PrinterMock pm;

  hx::Interpreter interpreter(&hsr, &itps, hx::ByteOrder::BE);
  interpreter.performInterpretation(&pm);

  REQUIRE(pm.events_.size() == 6);
  REQUIRE(pm.events_.front().type_ == hx::PrintEvent::Type::PE_START); pm.events_.pop();

  REQUIRE(pm.events_.front().type_ == hx::PrintEvent::Type::PE_PRINT_INTERPRETATION);
  REQUIRE(pm.events_.front().interpretation_.type_ == hx::Interpretation::UINT8);
  REQUIRE(pm.events_.front().interpretation_.size_ == 1);
  REQUIRE(pm.events_.front().hex_data_ == "01");
  REQUIRE(pm.events_.front().interpreted_str_ == "1");
  pm.events_.pop();

  REQUIRE(pm.events_.front().type_ == hx::PrintEvent::Type::PE_PRINT_INTERPRETATION);
  REQUIRE(pm.events_.front().interpretation_.type_ == hx::Interpretation::UINT8);
  REQUIRE(pm.events_.front().interpretation_.size_ == 1);
  REQUIRE(pm.events_.front().hex_data_ == "02");
  REQUIRE(pm.events_.front().interpreted_str_ == "2");
  pm.events_.pop();

  REQUIRE(pm.events_.front().type_ == hx::PrintEvent::Type::PE_PRINT_INTERPRETATION);
  REQUIRE(pm.events_.front().interpretation_.type_ == hx::Interpretation::UINT8);
  REQUIRE(pm.events_.front().interpretation_.size_ == 1);
  REQUIRE(pm.events_.front().hex_data_ == "03");
  REQUIRE(pm.events_.front().interpreted_str_ == "3");
  pm.events_.pop();

  REQUIRE(pm.events_.front().type_ == hx::PrintEvent::Type::PE_PRINT_INTERPRETATION);
  REQUIRE(pm.events_.front().interpretation_.type_ == hx::Interpretation::UINT8);
  REQUIRE(pm.events_.front().interpretation_.size_ == 1);
  REQUIRE(pm.events_.front().hex_data_ == "04");
  REQUIRE(pm.events_.front().interpreted_str_ == "4");
  pm.events_.pop();

  REQUIRE(pm.events_.front().type_ == hx::PrintEvent::Type::PE_END); pm.events_.pop();
}


TEST_CASE("Printing all types (From Big Endian Parsing)", "[Interpreter]") {
  GIVEN("A all interpreting components and byte order is BE") {
    hx::HexStringReader hsr("010000000000000000");
    hx::Interpretation i;
    std::vector<hx::Interpretation> itps;

    hx::PrinterMock pm;
    hx::Interpreter interpreter(&hsr, &itps, hx::ByteOrder::BE);

    WHEN("Interpretation is UINT8") {
      i.type_ = hx::Interpretation::UINT8;
      i.size_ = 1;
      itps.push_back(i);
      interpreter.performInterpretation(&pm);
      THEN("The value is 1") {
        REQUIRE(pm.events_.size() == 3);
        REQUIRE(pm.events_.front().type_ == hx::PrintEvent::Type::PE_START); pm.events_.pop();

        REQUIRE(pm.events_.front().type_ == hx::PrintEvent::Type::PE_PRINT_INTERPRETATION);
        REQUIRE(pm.events_.front().interpretation_.type_ == hx::Interpretation::UINT8);
        REQUIRE(pm.events_.front().interpretation_.size_ == 1);
        REQUIRE(pm.events_.front().hex_data_ == "01");
        REQUIRE(pm.events_.front().interpreted_str_ == "1");
        pm.events_.pop();

        REQUIRE(pm.events_.front().type_ == hx::PrintEvent::Type::PE_END); pm.events_.pop();
      }
    }
    WHEN("Interpretation is UINT16") {
      i.type_ = hx::Interpretation::UINT16;
      i.size_ = 2;
      itps.push_back(i);
      interpreter.performInterpretation(&pm);
      THEN("The value is 256") {
        REQUIRE(pm.events_.size() == 3);
        REQUIRE(pm.events_.front().type_ == hx::PrintEvent::Type::PE_START); pm.events_.pop();

        REQUIRE(pm.events_.front().type_ == hx::PrintEvent::Type::PE_PRINT_INTERPRETATION);
        REQUIRE(pm.events_.front().interpretation_.type_ == hx::Interpretation::UINT16);
        REQUIRE(pm.events_.front().interpretation_.size_ == 2);
        REQUIRE(pm.events_.front().hex_data_ == "0100");
        REQUIRE(pm.events_.front().interpreted_str_ == "256");
        pm.events_.pop();

        REQUIRE(pm.events_.front().type_ == hx::PrintEvent::Type::PE_END); pm.events_.pop();
      }
    }
    WHEN("Interpretation is UINT32") {
      i.type_ = hx::Interpretation::UINT32;
      i.size_ = 4;
      itps.push_back(i);
      interpreter.performInterpretation(&pm);
      THEN("The value is 256") {
        REQUIRE(pm.events_.size() == 3);
        REQUIRE(pm.events_.front().type_ == hx::PrintEvent::Type::PE_START); pm.events_.pop();

        REQUIRE(pm.events_.front().type_ == hx::PrintEvent::Type::PE_PRINT_INTERPRETATION);
        REQUIRE(pm.events_.front().interpretation_.type_ == hx::Interpretation::UINT32);
        REQUIRE(pm.events_.front().interpretation_.size_ == 4);
        REQUIRE(pm.events_.front().hex_data_ == "01000000");
        REQUIRE(pm.events_.front().interpreted_str_ == "16777216");
        pm.events_.pop();

        REQUIRE(pm.events_.front().type_ == hx::PrintEvent::Type::PE_END); pm.events_.pop();
      }
    }
  } // GIVEN
}
