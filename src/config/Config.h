#pragma once

#include <string>
#include <vector>

#include "data/ByteOrder.h"
#include "parse/Interpretation.h"

/*
  This class is merely a config parser and holder. It is constructed using
  the command line arguments passed when running the binary. Any unexpected
  value will cause the process to print usage and exit.
*/
namespace hx {

class Config {
public: //---------------- Public interface -------------------
  Config(int argc, char* argv[]);

  // Print the content of the config
  void print() const;

  // Validate the config
  bool validate() const;

  Config()                        = delete;
  Config(const Config&)           = delete;
  Config& operator=(const Config) = delete;

  // Input data
  std::string                   hex_string_;          // Input data as a hexadecimal string, e.g. "A052F1C8" (not case sensitive)
  std::string                   filepath_;            // Path to a file containing the data to interpret
  //TODO: ensure that hex_string_ and filepath_ are mutually exclusive

  // Offset when looking at a file
  size_t                        offset_;              // If input is a file, this is the offset at which the interpretation begins. 0 by default.

  // Translation options
  size_t                        padding_;             // How the data is padded (must be a power of 2) 0 means tightly packed. Tightly packed by default.
  ByteOrder::Enum               byte_order_;          // Endianness, big endian by default
  std::vector<Interpretation>   interpretations_;     // Vector of interpretations, determining how the input data should be interpreted
  // If empty, all compatible interpretations are tested on the first bytes of the data
};

}; //namespace scx
