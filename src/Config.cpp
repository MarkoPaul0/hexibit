#include "Config.h"
#include <getopt.h>
#include <stdexcept>
#include <cstring>
#include <set>
#include <errno.h>
#include "UtilDefs.h"

namespace hx {

static std::string toUpper(const std::string& str) {
  std::string res;
  const char* p = str.c_str();
  while (*p != 0) {
    res += static_cast<char>(::toupper(*p));
    ++p;
  }

  return res;
}


static bool cstrToByteOrder(const char* byte_order_str, ByteOrder* byte_order_out) {
  const std::string bo(byte_order_str);
  const std::string upper_bo = toUpper(bo);

  bool valid = true;
  if (upper_bo == "LE" || bo == "LITTLE_ENDIAN")
    *byte_order_out = ByteOrder::LE;
  else if (upper_bo == "BE" || bo == "LITTLE_ENDIAN")
    *byte_order_out = ByteOrder::BE;
  else
    valid = false;

  return valid;
}

static const char* byteOrderToCstr(ByteOrder bo) {
  switch(bo) {
    case ByteOrder::LE: return "LITTLE_ENDIAN";
    case ByteOrder::BE: return "BIG_ENDIAN";
    default:            _DEATH("Unknown byte order"); //Add %d
  }
}

static const char* interpretationToCstr(Interpretation itp) {
  switch (itp) {
    case Interpretation::UINT8:     return "UINT8";
    case Interpretation::UINT16:    return "UINT16";
    case Interpretation::UINT32:    return "UINT32";
    case Interpretation::UINT64:    return "UINT64";
    case Interpretation::INT8:      return "INT8";
    case Interpretation::INT16:     return "INT16";
    case Interpretation::INT32:     return "INT32";
    case Interpretation::INT64:     return "INT64";
    case Interpretation::DOUBLE:    return "DOUBLE";
    case Interpretation::BOOL:      return "BOOL";
    case Interpretation::STRING:    return "STRING";
    case Interpretation::IPV4:      return "IPV4";
    case Interpretation::SKIPPED:   return "SKIPPED";
    default:                        _DEATH("Unknown intepretation"); //Add %d
  }
}


static bool strToInterpretation(const std::string& interpretation_str, Interpretation* interpretation_out) {
  std::string up_str = toUpper(interpretation_str);
  bool valid = true;
  if (up_str == "UINT8") {
    *interpretation_out = Interpretation::UINT8;
  } else if (up_str == "UINT16") {
    *interpretation_out = Interpretation::UINT16;
  } else if (up_str == "UINT32") {
    *interpretation_out = Interpretation::UINT32;
  } else if (up_str == "UINT64") {
    *interpretation_out = Interpretation::UINT64;
  } else if (up_str == "INT8") {
    *interpretation_out = Interpretation::INT8;
  } else if (up_str == "INT16") {
    *interpretation_out = Interpretation::INT16;
  } else if (up_str == "INT32") {
    *interpretation_out = Interpretation::INT32;
  } else if (up_str == "INT64") {
    *interpretation_out = Interpretation::INT64;
  } else if (up_str == "DOUBLE") {
    *interpretation_out = Interpretation::DOUBLE;
  } else if (up_str == "BOOL") {
    *interpretation_out = Interpretation::BOOL;
  } else if (up_str == "STRING") {
    *interpretation_out = Interpretation::STRING;
  } else if (up_str == "IPV4") {
    *interpretation_out = Interpretation::IPV4;
  } else {
    valid = false;
  }

  return valid;
}

uint64_t cstrToUInt64(const char* str) {
  char* endptr;
  errno = 0;
  unsigned long long val = strtoull(str, &endptr, /*base*/10);
  if (errno != 0 || endptr == str || *endptr != '\0') { //TODO: verify these conditions
    _DEATH("Expected uint64, got %s", str);
  }
  return static_cast<uint64_t>(val);
}


static bool cstrToInterpretations(const char* interpretations_str, std::vector<Interpretation>* interpretations_out) {
  //_INFO("Trying to translate %s", interpretations_str);
  const char* p = interpretations_str;
  const char* cur_str = p;
  size_t cur_len = 0;

  //TODO: refactor this method

  while (true) {
    cur_len++;
    if (*p == ',' || *p == '\0') {
      const std::string str(cur_str, cur_len - 1);
      cur_len = 0;
      if (*p == ',') {
        cur_str = p + 1;
      }
      Interpretation interp;
      if (!strToInterpretation(str, &interp)) {
        _DEATH("Invalid interpretation '%s'", str.c_str());
      }

      interpretations_out->push_back(interp);
    }
    if (*p == '\0')
      break;
    ++p;
  }

  return true; //TODO: useless?
}


Config::Config(int argc, char* argv[]) {
  static option long_options[] = {
    {"hex-string",          required_argument, 0, 's'},
    {"file",                required_argument, 0, 'f'},
    {"reinterpret-as",      required_argument, 0, 'r'},
    {"padding",             required_argument, 0, 'p'},
    {"byte-order",          required_argument, 0, 'b'},
    {"all-interpretations", no_argument,       0, 'a'},
    {"offset",              required_argument, 0, 'o'},
    {"num-bytes",           required_argument, 0, 'n'}
  };

  int c = 0;
  std::set<int> opt_seen;
  while (c >= 0) {
    int opt_index = 0;
    if ((c = getopt_long_only(argc, argv, "s:f:r:p:b:ao:n:", long_options, &opt_index)) < 0) {
      break;//end of options;
    } else if (c == '?' || c == ':') {
      _DEATH("Invalid arguments");
    }

    if (!opt_seen.insert(c).second) {
      _DEATH("Option %s is provided twice!", optarg);
    }

    switch (c)
    {
    case 's': {  // Hexadecimal string
      hex_string_ = std::string(optarg);
      break;
    }
    case 'f': {  // Filepath
      filepath_ = std::string(optarg);
      break;
    }
    case 'r': {
      cstrToInterpretations(optarg, &interpretations_);
      break;
    }
    case 'p': {
      padding_ = cstrToUInt64(optarg);
      if (padding_ != 0 && padding_ != 2 && padding_ != 4 && padding_ != 8) {
        _DEATH("'--padding' value must be one of 0, 2, 4, or 8!");
      }
      break;
    }
    case 'b': {
      if (!cstrToByteOrder(optarg, &byte_order_))
        _DEATH("Unknown byte order %s. Expecting one of {'LE', 'LITTLE_ENDIAN', 'BE', 'BIG_ENDIAN'} (not case sensitive)", optarg);
      break;
    }
    case 'a': {
      interpret_all_ = true;
      break;
    }
    case 'o': {
      offset_ = cstrToUInt64(optarg);
      break;
    }
    default:
      _DEATH("[Config] invalid option %d", c);
    }
  }

  //If there are unknown options
  if (optind < argc) {
    _DEATH("[Config] Invalid input");
  }

  //for (size_t i = 0; i < opt_size; ++i) {
  //  if (!opt_seen[i]) {
  //    _DEATH("[Config] missing argument '%s'!", long_options[i].name);
  //}
};

void Config::print() const {
  std::string interpretations_str;
  if (interpret_all_) {
    interpretations_str = "All";
  } else {
    for (Interpretation ipt: interpretations_) {
      if (!interpretations_str.empty())
        interpretations_str += ",";

      interpretations_str += interpretationToCstr(ipt);
    }
  }

  printf("----------------------------------------------------\n");
  printf("Configuration: hex-string: %s\n"
         "               filepath: %s\n"
         "               byte-order: %s\n"
         "               padding: %lu\n"
         "               interpretations: %s\n"
         "               offset: %lu\n",
         hex_string_.c_str(), filepath_.c_str(), byteOrderToCstr(byte_order_), padding_, interpretations_str.c_str(), offset_);
  printf("----------------------------------------------------\n");
}

} //namespace scx
