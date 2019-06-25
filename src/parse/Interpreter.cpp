#include "parse/Interpreter.h"

#include "MainUtils.h"

namespace hx {

bool isByteOrderSwappingNeeded(ByteOrder::Enum target_order) {
  union {
    uint16_t  value_;   // 16 bit unsigned integer
    char      mem_[2];  // underlying memory
  } uint16;

  uint16.value_ = 0x0102;
  const bool is_host_big_endian = (uint16.mem_[0] == 0x01);
  const bool is_target_order_big_endian = (target_order == ByteOrder::Enum::BE);

  return (is_host_big_endian != is_target_order_big_endian);
}


//TODO: refactor
uint64_t swapByteOrder64(uint64_t input) {
  uint64_t swapped;
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
    sprintf(&str_buf[2*i], "%02X", data[i]);

  std::string str(str_buf, str_len);
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


Interpreter::Interpreter(IDataReader* data_reader, const std::vector<Interpretation>* interpretations, ByteOrder::Enum target_byte_order) :
data_reader_(data_reader), interpretations_(interpretations), swap_byte_order_(isByteOrderSwappingNeeded(target_byte_order)) {
}


void Interpreter::performInterpretation(IConsolePrinter* printer) {
  printer->printHeader();

  for (hx::Interpretation itp: *interpretations_) {
    if (data_reader_->getRemainingLength() < itp.size_) {
      break; // TODO add log if remaining length is not 0
    }
    switch (itp.type_) {
      case hx::Interpretation::UINT8: {
        const char* data = data_reader_->getData(itp.size_);
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, std::to_string(dataToUInt8(data)));
        break;
      }
      case hx::Interpretation::UINT16: {
        const char* data = data_reader_->getData(itp.size_);
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, std::to_string(dataToUInt16(data, swap_byte_order_)));
        break;
      }
      case hx::Interpretation::UINT32: {
        const char* data = data_reader_->getData(itp.size_);
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, std::to_string(dataToUInt32(data, swap_byte_order_)));
        break;
      }
      case hx::Interpretation::UINT64: {
        const char* data = data_reader_->getData(itp.size_);
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, std::to_string(dataToUInt64(data, swap_byte_order_)));
        break;
      }
      case hx::Interpretation::INT8: {
        const char* data = data_reader_->getData(itp.size_);
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, std::to_string(dataToInt8(data)));
        break;
      }
      case hx::Interpretation::INT16: {
        const char* data = data_reader_->getData(itp.size_);
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, std::to_string(dataToInt16(data, swap_byte_order_)));
        break;
      }
      case hx::Interpretation::INT32: {
        const char* data = data_reader_->getData(itp.size_);
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, std::to_string(dataToInt32(data, swap_byte_order_)));
        break;
      }
      case hx::Interpretation::INT64: {
        const char* data = data_reader_->getData(itp.size_);
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, std::to_string(dataToInt64(data, swap_byte_order_)));
        break;
      }
      case hx::Interpretation::DOUBLE: {
        const char* data = data_reader_->getData(itp.size_);
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, std::to_string(dataToDouble(data, swap_byte_order_)));
        break;
      }
      case hx::Interpretation::BOOL: {
        const char* data = data_reader_->getData(itp.size_);
        const char* val_str = (dataToBool(data) ? "true" : "false");
        printer->printInterpretation(dataToHexString(data, itp.size_), itp, val_str);
        break;
      }
      case hx::Interpretation::STRING: {
        //const char* end_ptr = reinterpret_cast<const char*>(memchr(data_reader_->getData(), '\0', data_reader_->getRemainingLength()));
        //itp.size_ = static_cast<size_t>(end_ptr - data_reader_->getData() + 1);
        //const std::string str(data_reader_->getData(), itp.size_);
        //printer->printInterpretation(data_reader_->getDataAsHexString(itp.size_), itp, str);
        break;
      }
      case hx::Interpretation::CHAR_ARRAY: {
        //const std::string str(data_reader_->getData(), itp.size_);
        //printer->printInterpretation(data_reader_->getDataAsHexString(itp.size_), itp, str);
        break;
      }
      case hx::Interpretation::IPV4: {
        //const std::string ip_str = intIpv4ToStr(data_reader_->getUInt32());
        //printer->printInterpretation(data_reader_->getDataAsHexString(itp.size_), itp, ip_str);
        break;
      }
      case hx::Interpretation::SKIPPED: {
        //printer->printInterpretation(data_reader_->getDataAsHexString(itp.size_), itp, /*interpreted_value*/""); // We skip the data, so there is no interpreted value
        break;
      }
      default: _DEATH("Unknown intepretation of type '%d'", static_cast<int>(itp.type_));
    }
    data_reader_->advanceReadPtr(itp.size_);
  }

  printer->printFooter();
}


} // namespace hx
