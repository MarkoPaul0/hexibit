#include "Config.h"
#include <getopt.h>
#include <stdexcept>
#include <cstring>
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
    case Interpretation::IP:        return "IP";
    case Interpretation::SKIPPED:   return "SKIPPED";
    default:                        _DEATH("Unknown intepretation"); //Add %d
  }
}


static bool strToInterpretation(const std::string& interpretation_str, Interpretation* interpretation_out) {
  std::string up_str = toUpper(interpretation_str);
  bool valid = true;
  if (up_str == "UINT8") {
    *interpretation_out = Interpretation::UINT8;
  } else {
    valid = false;
  }

  return valid;
}


static bool cstrToInterpretations(const char* interpretations_str, std::vector<Interpretation>* interpretations_out) {
  const char* p = interpretations_str;
  const char* cur_str = p;
  size_t cur_len = 0;
  do {
    cur_len++;
    if (*p == ',' || *p == '\0') {
      const std::string str(cur_str, cur_len);
      Interpretation interp;
      if (!strToInterpretation(str, &interp))
        _DEATH("Invalid interpretation %s", str.c_str());

      interpretations_out->push_back(interp);
    }
  } while (*p != '\0');

  return true; //TODO: useless?
}


Config::Config(int argc, char* argv[]) {
  const size_t opt_size = 8;
  static option long_options[opt_size] = {
    {"hex-string",          required_argument, 0, 's'},
    {"file",                required_argument, 0, 'f'},
    {"reinterpret-as",      required_argument, 0, 'r'},
    {"padding",             required_argument, 0, 'p'},
    {"byte-order",          required_argument, 0, 'b'},
    {"all-interpretations", no_argument,       0, 'a'},
    {"offset",              required_argument, 0, 'o'},
    {"num-bytes",           required_argument, 0, 'n'}
  };

  bool opt_seen[opt_size] = {false, false, false, false, false, false, false, false};
  int c = 0;
  while (c >= 0) {
    int opt_index = 0;
    if ((c = getopt_long_only(argc, argv, "s:f:r:p:b:a:o:n:", long_options, &opt_index)) < 0) {
      break;//end of options;
    }
    if (opt_seen[opt_index]) {
      _DEATH("Option %s is provided twice!", optarg);
    }
    opt_seen[opt_index] = true;

    switch (c)
    {
    case 's': {  // Hexadecimal string
      hex_str_ = std::string(optarg);
      //ASSERT(!hex_str.empty());
      break;
    }
    case 'f': {  // Filepath
      filepath_ = std::string(optarg);
      //ASSERT(!filepath_.empty());
      break;
    }
    case 'r': {
      cstrToInterpretations(optarg, &interpretations_);
      break;
    }
    case 'p': {
      _DEATH("'--padding is not supported yet'");
      break;
    }
    case 'b': {
      if (!cstrToByteOrder(optarg, &byte_order_))
        _DEATH("Unknown byte order %s. Expecting one of {'LE', 'LITTLE_ENDIAN', 'BE', 'BIG_ENDIAN'} (not case sensitive)", optarg);
      break;
    }
    case 'a': {
      break;
    }
    case 'o': {
      break;
    }
    case 'n': {
      break;
    }
    default:
      _DEATH("[Config] invalid option %d", c);
    }
  }

  //If there are unknown options
  if (optind < argc) {
    while (optind < argc)
      _ERROR("[Config] unkown argument %s", argv[optind++]);

    _DEATH("[Config] Invalid input");
  }

  //for (size_t i = 0; i < opt_size; ++i) {
  //  if (!opt_seen[i]) {
  //    _DEATH("[Config] missing argument '%s'!", long_options[i].name);
  //}
};

void Config::print() const {
  std::string interpretations_str;
  for (Interpretation ipt: interpretations_) {
    if (!interpretations_str.empty())
      interpretations_str += ",";

    interpretations_str += interpretationToCstr(ipt);
  }

  _INFO("Configuration: \n"
        "\t hex-string: %s"
        "\t filepath: %s"
        "\t byte-order: %s"
        "\t interpretations: %s", hex_str_.c_str(), filepath_.c_str(), byteOrderToCstr(byte_order_), interpretations_str.c_str());
}

} //namespace scx
