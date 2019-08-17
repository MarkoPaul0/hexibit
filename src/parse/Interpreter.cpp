#include "parse/Interpreter.h"

#include <arpa/inet.h>
#include <cstddef>

#include "MainUtils.h"

namespace hx {

bool isByteOrderSwappingNeeded(ByteOrder::Enum target_order) {
  union {
    uint16_t  value_;   // 16 bit unsigned integer
    char      mem_[2];  // Underlying memory
  } uint16;

  uint16.value_ = 0x0102;

  const bool is_host_big_endian         = (uint16.mem_[0] == 0x01);
  const bool is_target_order_big_endian = (target_order == ByteOrder::Enum::BE);

  return (is_host_big_endian != is_target_order_big_endian);
}


uint64_t swapByteOrder64(uint64_t input) {
  uint64_t swapped = 0;
  uint8_t* data = reinterpret_cast<uint8_t*>(&swapped);

  data[0] = static_cast<uint8_t>(input >> 56);
  data[1] = static_cast<uint8_t>(input >> 48);
  data[2] = static_cast<uint8_t>(input >> 40);
  data[3] = static_cast<uint8_t>(input >> 32);
  data[4] = static_cast<uint8_t>(input >> 24);
  data[5] = static_cast<uint8_t>(input >> 16);
  data[6] = static_cast<uint8_t>(input >> 8);
  data[7] = static_cast<uint8_t>(input >> 0);

  return swapped;
}


static std::string dataToHexString(const char* data, size_t num_bytes) {
  const size_t str_len = 2*num_bytes;
  char* str_buf = new char[str_len];
  for(size_t i = 0; i < num_bytes; ++i)
    sprintf(&str_buf[2*i], "%02X", static_cast<uint8_t>(data[i]));

  std::string str(str_buf, str_len);
  delete [] str_buf;
  return str;
}


static uint64_t dataToUInt64(const char* data, bool swap_byte_order) {
  uint64_t value = *reinterpret_cast<const uint64_t*>(data);
  if (swap_byte_order)
    value = swapByteOrder64(value);

  return value;
}


static uint32_t dataToUInt32(const char* data, bool swap_byte_order) {
  uint32_t value = *reinterpret_cast<const uint32_t*>(data);
  if (swap_byte_order)
    value = htonl(value);

  return value;
}


static uint16_t dataToUInt16(const char* data, bool swap_byte_order) {
  uint16_t value = *reinterpret_cast<const uint16_t*>(data);
  if (swap_byte_order)
    value = htons(value);

  return value;
}


static uint8_t dataToUInt8(const char* data) {
  return *reinterpret_cast<const uint8_t*>(data);
}


static int64_t dataToInt64(const char* data, bool swap_byte_order) {
  uint64_t unsigned_value = dataToUInt64(data, swap_byte_order);
  return reinterpret_cast<int64_t&>(unsigned_value);
}


static int32_t dataToInt32(const char* data, bool swap_byte_order) {
  uint32_t unsigned_value = dataToUInt32(data, swap_byte_order);
  return reinterpret_cast<int32_t&>(unsigned_value);
}


static int16_t dataToInt16(const char* data, bool swap_byte_order) {
  uint16_t unsigned_value = dataToUInt16(data, swap_byte_order);
  return reinterpret_cast<int16_t&>(unsigned_value);
}


static int8_t dataToInt8(const char* data) {
  return *reinterpret_cast<const int8_t*>(data);
}


static double dataToDouble(const char* data, bool swap_byte_order) {
  uint64_t unsigned_value = dataToUInt64(data, swap_byte_order);
  return reinterpret_cast<double&>(unsigned_value);
}


static bool dataToBool(const char* data) {
  return *reinterpret_cast<const bool*>(data);
}


static std::string dataToIPV4String(const char* data, bool swap_byte_order) {
  union {
    uint32_t ip_as_uint_;
    uint8_t  octets_[4];
  } ip;

  ip.ip_as_uint_ = *reinterpret_cast<const uint32_t*>(data);

  if (swap_byte_order)
    ip.ip_as_uint_ = htonl(ip.ip_as_uint_);

  char ip_cstr[16];
  const int len = snprintf(ip_cstr, sizeof(ip_cstr), "%u.%u.%u.%u", ip.octets_[3], ip.octets_[2], ip.octets_[1], ip.octets_[0]);
  if (len < 7 || len > 15) // shortest ip is 0.0.0.0 longest is 255.255.255.255
    _DEATH("Error while stringifying integer ipv4!");

  return std::string(ip_cstr);
}


Interpreter::Interpreter(IDataReader* data_reader, const std::vector<Interpretation>* interpretations, ByteOrder::Enum target_byte_order) :
data_reader_(data_reader), interpretations_(interpretations), swap_byte_order_(isByteOrderSwappingNeeded(target_byte_order)) {
}


void Interpreter::performInterpretation(IConsolePrinter* printer) {
  printer->startPrint();

  for (hx::Interpretation itp: *interpretations_) {
    if (data_reader_->getRemainingLength() < itp.size_ || data_reader_->getRemainingLength() == 0) {
      _DEBUG("There is not enough data left to interpret");
      break;
    }
    switch (itp.type_) {
      case hx::Interpretation::UINT8: {
        const char* data = data_reader_->getReadPtr(itp.size_);
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, std::to_string(dataToUInt8(data)));
        break;
      }
      case hx::Interpretation::UINT16: {
        const char* data = data_reader_->getReadPtr(itp.size_);
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, std::to_string(dataToUInt16(data, swap_byte_order_)));
        break;
      }
      case hx::Interpretation::UINT32: {
        const char* data = data_reader_->getReadPtr(itp.size_);
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, std::to_string(dataToUInt32(data, swap_byte_order_)));
        break;
      }
      case hx::Interpretation::UINT64: {
        const char* data = data_reader_->getReadPtr(itp.size_);
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, std::to_string(dataToUInt64(data, swap_byte_order_)));
        break;
      }
      case hx::Interpretation::INT8: {
        const char* data = data_reader_->getReadPtr(itp.size_);
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, std::to_string(dataToInt8(data)));
        break;
      }
      case hx::Interpretation::INT16: {
        const char* data = data_reader_->getReadPtr(itp.size_);
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, std::to_string(dataToInt16(data, swap_byte_order_)));
        break;
      }
      case hx::Interpretation::INT32: {
        const char* data = data_reader_->getReadPtr(itp.size_);
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, std::to_string(dataToInt32(data, swap_byte_order_)));
        break;
      }
      case hx::Interpretation::INT64: {
        const char* data = data_reader_->getReadPtr(itp.size_);
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, std::to_string(dataToInt64(data, swap_byte_order_)));
        break;
      }
      case hx::Interpretation::DOUBLE: {
        const char* data = data_reader_->getReadPtr(itp.size_);
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, std::to_string(dataToDouble(data, swap_byte_order_)));
        break;
      }
      case hx::Interpretation::BOOL: {
        const char* data = data_reader_->getReadPtr(itp.size_);
        const char* val_str = (dataToBool(data) ? "true" : "false");
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, val_str);
        break;
      }
      case hx::Interpretation::CHAR_ARRAY: {
        if (itp.size_ == 0) {
          std::string str;
          std::vector<char> data_copy;
          while (data_reader_->getRemainingLength()) {
            const char c = *data_reader_->getReadPtr(/*num_bytes*/1);
            str += c;
            data_copy.push_back(c);
            data_reader_->advanceReadPtr(1);
            if (c == '\0')
              break;
          }
          itp.size_ = str.size();
          printer->printInterpretation(dataToHexString(data_copy.data(), itp.size_), itp, str);
        } else {
          const char* data = data_reader_->getReadPtr(itp.size_);
          const std::string str(data, itp.size_);
          printer->printInterpretation(dataToHexString(data, itp.size_), itp, str);
        }
        break;
      }
      case hx::Interpretation::IPV4: {
        const char* data = data_reader_->getReadPtr(itp.size_);
        const std::string ip_str = dataToIPV4String(data, swap_byte_order_);
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, ip_str);
        break;
      }
      case hx::Interpretation::SKIPPED: {
        const char* data = data_reader_->getReadPtr(itp.size_);
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, /*interpreted_value*/""); // We skip the data, so there is no interpreted value
        break;
      }
      default: _DEATH("Unknown intepretation of type '%d'", static_cast<int>(itp.type_));
    }
    data_reader_->advanceReadPtr(itp.size_);
  }

  printer->endPrint();
}

} // namespace hx
