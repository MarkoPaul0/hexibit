#pragma once

#include <string>
#include <vector>

#include "data/ByteOrder.h"
#include "parse/Interpretation.h"

/*
   This class is merely a config parser and holder. It is constructed using
   the command line arguments passed when running the binary. Any unexpected
   value will cause the process to exit
*/
namespace hx {

class Config {
public: //---------------- Public interface -------------------
  //Config(); // Only here so that I can get unit test going fast
  Config(int argc, char* argv[]);

  void print() const; // Prints the content of the config

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
  bool                          interpret_all_;       // If enabled, this will cause all possible viable interpretations to be performed on the starting bytes. Disabled by default.
};

}; //namespace scx
