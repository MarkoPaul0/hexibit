#include "Interpreter.h"

namespace hx {

static constexpr char PRINT_FORMAT[16] = "%-18s %-16s %s\n";

Interpreter::Interpreter(Buffer* buffer, const std::vector<Interpretation>* interpretations) : buffer_(buffer), interpretations_(interpretations) {
}

void Interpreter::performInterpretation() {
  printf(PRINT_FORMAT, "Data", "Interpretation", "Value");
  printf("-----------------------------------------------------------\n");
  //TODO: verify there is enough space left for the parsing
  for (hx::Interpretation itp: *interpretations_) {
    if (buffer_->getRemainingLength() < itp.size_) {
      break; // TODO add log if remaining length is not 0
    }
    switch (itp.type_) {
      case hx::Interpretation::UINT8:    {
        printf(PRINT_FORMAT, buffer_->getDataAsHexString(itp.size_).c_str(), Interpretation::interpretationToCstr(itp), std::to_string(buffer_->getUInt8()).c_str());
        break;
      }
      case hx::Interpretation::UINT16:   {
        printf(PRINT_FORMAT, buffer_->getDataAsHexString(itp.size_).c_str(), Interpretation::interpretationToCstr(itp), std::to_string(buffer_->getUInt16()).c_str());
        break;
      }
      case hx::Interpretation::UINT32:   {
        printf(PRINT_FORMAT, buffer_->getDataAsHexString(itp.size_).c_str(), Interpretation::interpretationToCstr(itp), std::to_string(buffer_->getUInt32()).c_str());
        break;
      }
      case hx::Interpretation::UINT64:   {
        printf(PRINT_FORMAT, buffer_->getDataAsHexString(itp.size_).c_str(), Interpretation::interpretationToCstr(itp), std::to_string(buffer_->getUInt64()).c_str());
        break;
      }
      case hx::Interpretation::INT8:    {
        printf(PRINT_FORMAT, buffer_->getDataAsHexString(itp.size_).c_str(), Interpretation::interpretationToCstr(itp), std::to_string(buffer_->getInt8()).c_str());
        break;
      }
      case hx::Interpretation::INT16:   {
        printf(PRINT_FORMAT, buffer_->getDataAsHexString(itp.size_).c_str(), Interpretation::interpretationToCstr(itp), std::to_string(buffer_->getInt16()).c_str());
        break;
      }
      case hx::Interpretation::INT32:   {
        printf(PRINT_FORMAT, buffer_->getDataAsHexString(itp.size_).c_str(), Interpretation::interpretationToCstr(itp), std::to_string(buffer_->getInt32()).c_str());
        break;
      }
      case hx::Interpretation::INT64:   {
        printf(PRINT_FORMAT, buffer_->getDataAsHexString(itp.size_).c_str(), Interpretation::interpretationToCstr(itp), std::to_string(buffer_->getInt64()).c_str());
        break;
      }
      case hx::Interpretation::DOUBLE:   {
        printf(PRINT_FORMAT, buffer_->getDataAsHexString(itp.size_).c_str(), Interpretation::interpretationToCstr(itp), std::to_string(buffer_->getDouble()).c_str());
        break;
      }
      case hx::Interpretation::BOOL:     {
        const char* val_str = (buffer_->getBool() ? "true" : "false");
        printf(PRINT_FORMAT, buffer_->getDataAsHexString(itp.size_).c_str(), Interpretation::interpretationToCstr(itp), val_str);
        break;
      }
      case hx::Interpretation::STRING:   {
        break;
      }
      case hx::Interpretation::IPV4:       {
        break;
      }
      case hx::Interpretation::SKIPPED:  {
        break;
      }
      default:                        _DEATH("Unknown intepretation"); //Add %d
    }
    buffer_->advanceReadPtr(itp.size_);
  }
}


} // namespace hx
