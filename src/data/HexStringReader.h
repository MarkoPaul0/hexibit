#pragma once

#include <string>

#include "data/IDataReader.h"

namespace hx {
/*
  Small class implementing IDataReader functionalities by providing access to
  data from a hexadecimal string.
*/
class HexStringReader : public IDataReader {
public: //----------------------- Public Interface -----------------------------
  // Constructor
  HexStringReader(const std::string& hex_str);

  // Destructor
  ~HexStringReader();

  //--------------------- Implementation of IDataReader ------------------------
  // All of the functions below are an implementation of IDataReader.
  // Read IDataReader.h for more information
  void advanceReadPtr(size_t num_bytes) override;
  size_t getRemainingLength() const override;
  const char* getReadPtr(size_t num_bytes) override;

private: //--------------------- Private Interface -----------------------------
  const char* const       data_;   // Underlying data
  const size_t            len_;    // Length of data_
  size_t                  offset_; // Reading offset
};

} // namespace hx
