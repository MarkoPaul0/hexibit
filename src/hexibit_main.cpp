#include <stdexcept>
#include <csignal>

#include "UtilDefs.h"
#include "Config.h"
#include <getopt.h>
#include "Buffer.h"

static volatile sig_atomic_t stop_requested = 0;

void signalHandler(int signum) {
  _INFO("[HexibitApp] Received signum %d", signum);
  stop_requested = 1;
}

int main(int argc, char* argv[]) {
  hx::Config cfg(argc, argv);
  cfg.print();

  _INFO("[HexibitApp] exiting");
  return 0;
}
