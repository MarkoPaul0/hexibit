#include "parse/Interpreter.h"

#include "MainUtils.h"

namespace hx {

static std::string intIpv4ToStr(uint32_t ipv4_uint) {
  union {
    uint32_t ip_as_uint_;
    uint8_t  octets_[4];
  } ip;
  ip.ip_as_uint_ = ipv4_uint;

  char ip_cstr[16];
  const int len = snprintf(ip_cstr, sizeof(ip_cstr), "%u.%u.%u.%u", ip.octets_[3], ip.octets_[2], ip.octets_[1], ip.octets_[0]);
  if (len < 7 || len > 15) // shortest ip is 0.0.0.0 longest is 255.255.255.255
    _DEATH("Error while stringifying integer ipv4!");

  return std::string(ip_cstr);
}


Interpreter::Interpreter(IDataReader* data_reader, const std::vector<Interpretation>* interpretations) : data_reader_(data_reader), interpretations_(interpretations) {
}


void Interpreter::performInterpretation(IConsolePrinter* printer) {
  printer->printHeader();

  for (hx::Interpretation itp: *interpretations_) {
    if (data_reader_->getRemainingLength() < itp.size_) {
      break; // TODO add log if remaining length is not 0
    }
    switch (itp.type_) {
      case hx::Interpretation::UINT8: {
        printer->printInterpretation(data_reader_->getDataAsHexString(itp.size_), itp, std::to_string(data_reader_->getUInt8()));
        break;
      }
      case hx::Interpretation::UINT16: {
        printer->printInterpretation(data_reader_->getDataAsHexString(itp.size_), itp, std::to_string(data_reader_->getUInt16()));
        break;
      }
      case hx::Interpretation::UINT32: {
        printer->printInterpretation(data_reader_->getDataAsHexString(itp.size_), itp, std::to_string(data_reader_->getUInt32()));
        break;
      }
      case hx::Interpretation::UINT64: {
        printer->printInterpretation(data_reader_->getDataAsHexString(itp.size_), itp, std::to_string(data_reader_->getUInt64()));
        break;
      }
      case hx::Interpretation::INT8: {
        printer->printInterpretation(data_reader_->getDataAsHexString(itp.size_), itp, std::to_string(data_reader_->getInt8()));
        break;
      }
      case hx::Interpretation::INT16: {
        printer->printInterpretation(data_reader_->getDataAsHexString(itp.size_), itp, std::to_string(data_reader_->getInt16()));
        break;
      }
      case hx::Interpretation::INT32: {
        printer->printInterpretation(data_reader_->getDataAsHexString(itp.size_), itp, std::to_string(data_reader_->getInt32()));
        break;
      }
      case hx::Interpretation::INT64: {
        printer->printInterpretation(data_reader_->getDataAsHexString(itp.size_), itp, std::to_string(data_reader_->getInt64()));
        break;
      }
      case hx::Interpretation::DOUBLE: {
        printer->printInterpretation(data_reader_->getDataAsHexString(itp.size_), itp, std::to_string(data_reader_->getDouble()));
        break;
      }
      case hx::Interpretation::BOOL: {
        const char* val_str = (data_reader_->getBool() ? "true" : "false");
        printer->printInterpretation(data_reader_->getDataAsHexString(itp.size_), itp, val_str);
        break;
      }
      case hx::Interpretation::STRING: {
        const char* end_ptr = reinterpret_cast<const char*>(memchr(data_reader_->getData(), '\0', data_reader_->getRemainingLength()));
        itp.size_ = static_cast<size_t>(end_ptr - data_reader_->getData() + 1);
        const std::string str(data_reader_->getData(), itp.size_);
        printer->printInterpretation(data_reader_->getDataAsHexString(itp.size_), itp, str);
        break;
      }
      case hx::Interpretation::CHAR_ARRAY: {
        const std::string str(data_reader_->getData(), itp.size_);
        printer->printInterpretation(data_reader_->getDataAsHexString(itp.size_), itp, str);
        break;
      }
      case hx::Interpretation::IPV4: {
        const std::string ip_str = intIpv4ToStr(data_reader_->getUInt32());
        printer->printInterpretation(data_reader_->getDataAsHexString(itp.size_), itp, ip_str);
        break;
      }
      case hx::Interpretation::SKIPPED: {
        printer->printInterpretation(data_reader_->getDataAsHexString(itp.size_), itp, /*interpreted_value*/""); // We skip the data, so there is no interpreted value
        break;
      }
      default: _DEATH("Unknown intepretation of type '%d'", static_cast<int>(itp.type_));
    }
    data_reader_->advanceReadPtr(itp.size_);
  }

  printer->printFooter();
}


} // namespace hx
