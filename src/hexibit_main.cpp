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


static void interpretData(hx::Buffer* data, const std::vector<hx::Interpretation>& interpretations) {
  //TODO: verify there is enough space left for the parsing
  for (hx::Interpretation itp: interpretations) {
    switch (itp) {
      case hx::Interpretation::UINT8:    {
        uint8_t value = data->getUInt8();
        printf("UINT8: %u\n", value);
        data->advanceReadPtr(sizeof(value));
        break;
      }
      case hx::Interpretation::UINT16:   {
        uint16_t value = data->getUInt16();
        printf("UINT16: %u\n", value);
        data->advanceReadPtr(sizeof(value));
        break;
      }
      case hx::Interpretation::UINT32:   {
        uint32_t value = data->getUInt32();
        printf("UINT32: %u\n", value);
        data->advanceReadPtr(sizeof(value));
        break;
      }
      case hx::Interpretation::UINT64:   {
        uint64_t value = data->getUInt64();
        printf("UINT64: %llu\n", value);
        data->advanceReadPtr(sizeof(value));
        break;
      }
      case hx::Interpretation::INT8:    {
        int8_t value = data->getInt8();
        printf("INT8: %u\n", value);
        data->advanceReadPtr(sizeof(value));
        break;
      }
      case hx::Interpretation::INT16:   {
        int16_t value = data->getInt16();
        printf("INT16: %u\n", value);
        data->advanceReadPtr(sizeof(value));
        break;
      }
      case hx::Interpretation::INT32:   {
        int32_t value = data->getInt32();
        printf("INT32: %u\n", value);
        data->advanceReadPtr(sizeof(value));
        break;
      }
      case hx::Interpretation::INT64:   {
        int64_t value = data->getInt64();
        printf("INT64: %llu\n", value);
        data->advanceReadPtr(sizeof(value));
        break;
      }
      case hx::Interpretation::DOUBLE:   {
        double value = data->getDouble();
        printf("DOUBLE: %f\n", value);
        data->advanceReadPtr(sizeof(value));
        break;
      }
      case hx::Interpretation::BOOL:     {
        bool value = data->getBool();
        printf("BOOL: %s\n", (value ? "true" : "false"));
        data->advanceReadPtr(sizeof(value));
        break;
      }
      case hx::Interpretation::STRING:   {
        break;
      }
      case hx::Interpretation::IPV4:       {
        break;
      }
      case hx::Interpretation::SKIPPED:  {
        break;
      }
      default:                        _DEATH("Unknown intepretation"); //Add %d
    }
  }
}


int main(int argc, char* argv[]) {
  const hx::Config cfg(argc, argv);
  cfg.print();

  hx::Buffer buffer(cfg.byte_order_);
  if (!cfg.hex_string_.empty()) {
    buffer.initFromHexString(cfg.hex_string_);
  } else if (!cfg.filepath_.empty()) {
    buffer.initFromFile(cfg.filepath_, cfg.offset_);
  } else {
    _DEATH("Invalid config");
  }

  interpretData(&buffer, cfg.interpretations_);

  _INFO("[HexibitApp] exiting");
  return 0;
}
