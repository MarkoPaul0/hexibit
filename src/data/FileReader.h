#pragma once

#include <string>

#include "data/IDataReader.h"

namespace hx {
/*
  Small class implementing IDataReader functionalities by providing access to
  data from a file.
*/
class FileReader : public IDataReader {
public: //----------------------- Public Interface -----------------------------
  // Constructor
  // Inputs:
  //    filepath:         path to the file to be read.
  //    initial_offset:   offset at which the file starts being read.
  FileReader(const std::string& filepath, size_t initial_offset);

  // Destructor
  ~FileReader();

  //--------------------- Implementation of IDataReader ------------------------
  // All of the functions below are an implementation of IDataReader.
  // Read IDataReader.h for more information
  void advanceReadPtr(size_t num_bytes) override;
  size_t getRemainingLength() const override;
  const char* getReadPtr(size_t num_bytes) override;

private: //---------------------- Public Interface -----------------------------
  FILE*      file_;                   // File descriptor
  size_t     file_size_;              // Size of file_ in bytes
  char*      current_data_;           // Buffer used to temporarily place data extracted from the file
  size_t     current_data_capacity_;  // Size of current_data_
};


} // namespace hx
