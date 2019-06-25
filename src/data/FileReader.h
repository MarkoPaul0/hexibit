#pragma once

#include <string>

#include "data/IDataReader.h"

namespace hx {

class FileReader : public IDataReader {
public:
  // Constructor
  FileReader(const std::string& filepath, size_t initial_offset);

  // Destructor
  ~FileReader();

  void advanceReadPtr(size_t advance_size) override;

  size_t getRemainingLength() const override;

  const char* getData(size_t num_bytes) override;

private:
  FILE*      file_;
  size_t     file_size_;
  char*      current_data_; // Buffer used to temporarily place data extracted from the file, process it and expose it to the client
  size_t     current_data_capacity_;
};


} // namespace hx
