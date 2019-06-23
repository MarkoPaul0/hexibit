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
//  Config(); // Only here so that I can get unit test going fast
  Config(int argc, char* argv[]);

  void print() const; // Prints the content of the config

  Config()                        = delete;
  Config(const Config&)           = delete;
  Config& operator=(const Config) = delete;

  // Input data
  std::string                   hex_string_;
  std::string                   filepath_;   //TODO: ensure that hex_string_ and filepath_ are mutually exclusive

  // Offset when looking at a file
  size_t                        offset_;

  // Translation options
  size_t                        padding_;
  ByteOrder::Enum               byte_order_;
  std::vector<Interpretation>   interpretations_;
  bool                          interpret_all_;
};

}; //namespace scx
