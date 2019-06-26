#pragma once

namespace hx {

/*
  Abstract class exposing a simple data reading interface. It is meant to be used
  to abstract away the complexity of reading data from various sources.
  This abstraction views any data source as a collection of bytes, which is accessed
  by a read pointer. This read pointer can be advanced to allow traversal of the
  byte collection.
*/
class IDataReader {
public:
  virtual ~IDataReader() {
    // Empty virtual destructor
  }

  // This method returns the number of bytes remaining to be read in the underlying
  // data source.
  virtual size_t getRemainingLength() const = 0;

  // Advance the read pointer by num_bytes. Calling this method with num_bytes greater
  // than the value returned by getRemainingLength() is an error.
  virtual void advanceReadPtr(size_t num_bytes) = 0;

  // Method providing access to num_bytes of the underlying data. The returned pointer
  // is owned by the IDataReader instance and may be invalidated by subsequent calls
  // to getReadPtr(). It is invalid to call this method with num_bytes = 0.
  virtual const char* getReadPtr(size_t num_bytes) = 0;
};

} // namespace hx
