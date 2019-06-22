#pragma once

//#include <cstdint>
#include <string>
#include <vector>

#include "ByteOrder.h"
#include "Interpretation.h"

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


  std::string getHexString() const {
    return hex_str_;
  }

  std::string getFilepath() const {
    return filepath_;
  }

  ByteOrder getByteOrder() const {
    return byte_order_;
  }

  const std::vector<Interpretation>* getInterpretations() const {
    return &interpretations_;
  }


private: //---------------- Private interface -------------------
  Config()                        = delete;
  Config(const Config&)           = delete;
  Config& operator=(const Config) = delete;

  // Input data
  std::string                   hex_str_;
  std::string                   filepath_;   //TODO: ensure that hex_str_ and filepath_ are mutually exclusive

  // Data target
  size_t                        offset_;
  size_t                        num_bytes_;

  // Translation options
  ByteOrder                     byte_order_;
  std::vector<Interpretation>   interpretations_;
  bool                          interpret_all_;
};

}; //namespace scx
