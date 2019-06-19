#pragma once

#include <cstdint>
#include <string>

/*
    This class is merely a config parser and holder. It is constructed using 
    the command line arguments passed when running the binary. Any unexpected
    value will cause the process to exit
*/
namespace hx {

class Config {
public: //---------------- Public interface -------------------
    Config(); // Only here so that I can get unit test going fast
    Config(int argc, char* argv[]);

    Config(const Config&)           = delete;
    Config& operator=(const Config) = delete;

    void print() const; // Prints the content of the config
};

}; //namespace scx
