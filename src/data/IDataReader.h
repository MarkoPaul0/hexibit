#pragma once

#include <string>

namespace hx {

class IDataReader {
public:
  virtual ~IDataReader() {
    // Empty virtual destructor
  }

  virtual void advanceReadPtr(size_t advance_size) = 0;
  virtual size_t getRemainingLength() const = 0;

  virtual const char* getData() const = 0;

  //TODO:  Remove the need for copy
  virtual std::string getDataAsHexString(size_t num_bytes) const = 0;

  virtual uint64_t    getUInt64()     const = 0;
  virtual uint32_t    getUInt32()     const = 0;
  virtual uint16_t    getUInt16()     const = 0;
  virtual uint8_t     getUInt8()      const = 0;

  virtual int64_t     getInt64()      const = 0;
  virtual int32_t     getInt32()      const = 0;
  virtual int16_t     getInt16()      const = 0;
  virtual int8_t      getInt8()       const = 0;

  virtual double      getDouble()     const = 0;
  virtual bool        getBool()       const = 0;
};


} // namespace hx
