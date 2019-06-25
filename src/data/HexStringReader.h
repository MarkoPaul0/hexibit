#pragma once

#include <string>

#include "data/IDataReader.h"

namespace hx {

class HexStringReader : public IDataReader {
public:
  // Constructor
  HexStringReader(const std::string& hex_str);

  // Destructor
  ~HexStringReader();

  void advanceReadPtr(size_t advance_size) override;

  size_t getRemainingLength() const override;

  const char* getData(size_t num_bytes) override;

private:
  const char* const       data_;
  const size_t            len_;
  size_t                  offset_;
};

} // namespace hx
