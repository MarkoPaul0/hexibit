#include <stdexcept>
#include <csignal>

#include "UtilDefs.h"
#include "config/Config.h"
#include <getopt.h>
#include "Buffer.h"

static volatile sig_atomic_t stop_requested = 0;

void signalHandler(int signum) {
  _INFO("[HexibitApp] Received signum %d", signum);
  stop_requested = 1;
}

int main(int argc, char* argv[]) {
  const size_t opt_size = 1;
  static option long_options[opt_size] = {
    {"hex-string",  required_argument, 0, 's'}
  };

  bool opt_seen[opt_size] = {false};
  int c = 0;
  while (c >= 0) {
    int opt_index = 0;
    if ((c = getopt_long_only(argc, argv, "s:", long_options, &opt_index)) < 0) {
      break;//end of options;
    }
    if (opt_seen[opt_index]) {
      _DEATH("Option %s is provided twice!", optarg);
    }
    opt_seen[opt_index] = true;
    switch (c) {
      case 's': {  //SYMBOL
        std::string hex_data(optarg);
        hx::Buffer buf;
        buf.initFromHexString(hex_data);
        _INFO("Hex data: %s", hex_data.c_str());
        //_INFO("Parsing uint64: %llu", buf.getUInt64());
        break;
      }
      default:
        _DEATH("[Config] invalid option %d", c);
    }
  }

  //If there are unkown options
  if (optind < argc) {
    while (optind < argc) {
      _DEATH("[Config] unkown argument %s", argv[optind++]);
    }
  }

  for (size_t i = 0; i < opt_size; ++i) {
    if (!opt_seen[i]) {
      _DEATH("[Config] missing argument '%s'!", long_options[i].name);
    }
  }

  _INFO("[HexibitApp] exiting");
  return 0;
}
