#pragma once

namespace hx {

class IDataReader {
public:
  virtual ~IDataReader() {
    // Empty virtual destructor
  }

  virtual void advanceReadPtr(size_t advance_size) = 0;

  virtual size_t getRemainingLength() const = 0;

  // The returned data is owned by the reader and can be invalidated
  // be the next getXXXX() call
  virtual const char* getData(size_t num_bytes) const = 0;
};


} // namespace hx
