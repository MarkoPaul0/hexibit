#include "Interpreter.h"

namespace hx {


Interpreter::Interpreter(Buffer* buffer, const std::vector<Interpretation>* interpretations) : buffer_(buffer), interpretations_(interpretations) {
}


void Interpreter::performInterpretation(IConsolePrinter* printer) {
  printer->printHeader();

  for (hx::Interpretation itp: *interpretations_) {
    if (buffer_->getRemainingLength() < itp.size_) {
      break; // TODO add log if remaining length is not 0
    }
    switch (itp.type_) {
      case hx::Interpretation::UINT8: {
        printer->printInterpretation(buffer_->getDataAsHexString(itp.size_), itp, std::to_string(buffer_->getUInt8()));
        break;
      }
      case hx::Interpretation::UINT16: {
        printer->printInterpretation(buffer_->getDataAsHexString(itp.size_), itp, std::to_string(buffer_->getUInt16()));
        break;
      }
      case hx::Interpretation::UINT32: {
        printer->printInterpretation(buffer_->getDataAsHexString(itp.size_), itp, std::to_string(buffer_->getUInt32()));
        break;
      }
      case hx::Interpretation::UINT64: {
        printer->printInterpretation(buffer_->getDataAsHexString(itp.size_), itp, std::to_string(buffer_->getUInt64()));
        break;
      }
      case hx::Interpretation::INT8: {
        printer->printInterpretation(buffer_->getDataAsHexString(itp.size_), itp, std::to_string(buffer_->getInt8()));
        break;
      }
      case hx::Interpretation::INT16: {
        printer->printInterpretation(buffer_->getDataAsHexString(itp.size_), itp, std::to_string(buffer_->getInt16()));
        break;
      }
      case hx::Interpretation::INT32: {
        printer->printInterpretation(buffer_->getDataAsHexString(itp.size_), itp, std::to_string(buffer_->getInt32()));
        break;
      }
      case hx::Interpretation::INT64: {
        printer->printInterpretation(buffer_->getDataAsHexString(itp.size_), itp, std::to_string(buffer_->getInt64()));
        break;
      }
      case hx::Interpretation::DOUBLE: {
        printer->printInterpretation(buffer_->getDataAsHexString(itp.size_), itp, std::to_string(buffer_->getDouble()));
        break;
      }
      case hx::Interpretation::BOOL: {
        const char* val_str = (buffer_->getBool() ? "true" : "false");
        printer->printInterpretation(buffer_->getDataAsHexString(itp.size_), itp, val_str);
        break;
      }
      case hx::Interpretation::STRING: {
        break;
      }
      case hx::Interpretation::IPV4: {
        break;
      }
      case hx::Interpretation::SKIPPED: {
        break;
      }
      default: _DEATH("Unknown intepretation"); //Add %d
    }
    buffer_->advanceReadPtr(itp.size_);
  }

  printer->printFooter();
}


} // namespace hx
