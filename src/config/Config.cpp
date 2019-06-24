#include "config/Config.h"

#include <getopt.h>
#include <cstring>
#include <errno.h>
#include <set>

#include "MainUtils.h"

namespace hx {

static void printUsage(const char* bin_path) {
  printf("\n");
  printf("Usage:\n");
  printf("\t%s -s  <hex_string> [-i <interpretation,...> -p <padding> -b <byte_order>]\n", bin_path);
  printf("\t%s -f  <filepath> [-i <interpretation,...> -p <padding> -b <byte_order> -o <offset> -n <num_bytes>]\n", bin_path);
  printf("\n");
  printf("Where:\n");
  printf("\t<hex_string>     is a hexadecimal string (With or without whitespaces, not case sensitive)\n");
  printf("\t<interpretation> is one of uint[8|16|32|64], int[8|16|32|64], double, ipv4, string, char_array_<length>, bool, skipped_<length> (Not case sensitive)\n");
  printf("\t<padding>        is one of 0, 2, 4, or 8 (Defaulted to 0)\n");
  printf("\t<byte_order>     is one of LITTLE_ENDIAN, BIG_ENDIAN, LE, or BE. (Not case sensitive, defaulted to BE)\n");
  printf("\n");
  printf("\n");
  printf("All options have long name equivalents:\n");
  printf("\t-s or --hex-string\n");
  printf("\t-f or --filepath\n");
  printf("\t-i or --interpretations\n");
  printf("\t-p or --padding\n");
  printf("\t-b or --byte-order\n");
  printf("\t-o or --offset\n");
  printf("\t-n or --num-bytes\n");
  printf("\n");
  printf("Examples:\n");
  printf("\t%s -s  \"01 B2 Ff4c\" -i bool,uint8,int16\n", bin_path);
  printf("\t%s -s  \"0168656c6c6f20776f726c64\" -i bool,char_array_11\n", bin_path);
  printf("\t%s -f  some/file/to_analyze.txt -o 128 -n 4 -i ipv4]\n", bin_path);
  printf("\n");
  // TODO: add examples with skipped, char_array
}


static std::string toUpper(const std::string& str) {
  std::string res;
  const char* p = str.c_str();
  while (*p != 0) {
    res += static_cast<char>(::toupper(*p));
    ++p;
  }

  return res;
}


static std::string cleanHexString(const char* c_str) {
  std::string trimmed_str;
  const char* p = c_str;
  while (*p != '\0') {
    if (isspace(*p) == 0)
      trimmed_str += *p;
    ++p;
  }
  return trimmed_str;
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
  const char* p = interpretations_str;
  const char* cur_str = p;
  size_t cur_len = 0;

  //TODO: refactor this method
  while (true) {
    cur_len++;
    if (*p == ',' || *p == '\0') {
      std::string str(cur_str, cur_len - 1);
      str = toUpper(str);
      cur_len = 0;
      if (*p == ',') {
        cur_str = p + 1;
      }
      Interpretation interp;
      if (!Interpretation::strToInterpretation(str, &interp)) {
        _ERROR("Invalid interpretation '%s'", str.c_str());
        return false;
      }

      interpretations_out->push_back(interp);
    }
    if (*p == '\0')
      break;
    ++p;
  }

  return true;
}


Config::Config(int argc, char* argv[]) {
  static option long_options[] = {
    {"help",                no_argument,       0, 'h'},
    {"hex-string",          required_argument, 0, 's'},
    {"filepath",            required_argument, 0, 'f'},
    {"interpretations",     required_argument, 0, 'i'},
    {"padding",             required_argument, 0, 'p'},
    {"byte-order",          required_argument, 0, 'b'},
    {"offset",              required_argument, 0, 'o'},
    {"num-bytes",           required_argument, 0, 'n'}
  };

  int c = 0;
  std::set<int> opt_seen;
  while (c >= 0) {
    int opt_index = 0;
    if ((c = getopt_long_only(argc, argv, "hs:f:i:p:b:o:n:", long_options, &opt_index)) < 0) {
      break; // End of options;
    } else if (c == '?' || c == ':') {
      _ERROR("Invalid arguments!");
      printUsage(argv[0]);
      exit(1);
    }

    if (!opt_seen.insert(c).second) {
      _DEATH("Option %s is provided twice!", optarg);
    }

    switch (c)
    {
    case 'h': {
      printUsage(argv[0]);
      exit(0);
      break;
    }
    case 's': {  // Hexadecimal string
      hex_string_ = cleanHexString(optarg);
      //TODO: validate that this is a proper hexa string
      break;
    }
    case 'f': {  // Filepath
      filepath_ = std::string(optarg);
      break;
    }
    case 'i': {
      if (!cstrToInterpretations(optarg, &interpretations_)) {
        printUsage(argv[0]);
        exit(1);
      }
      break;
    }
    case 'p': {
      padding_ = cstrToUInt64(optarg);
      if (padding_ != 0 && padding_ != 2 && padding_ != 4 && padding_ != 8) {
        _ERROR("'--padding' value must be one of 0, 2, 4, or 8!");
        printUsage(argv[0]);
        exit(1);
      }
      break;
    }
    case 'b': {
      std::string arg = toUpper(optarg);
      if (!ByteOrder::cstrToByteOrder(arg.c_str(), &byte_order_)) {
        _ERROR("Unknown byte order %s. Expecting one of {'LE', 'LITTLE_ENDIAN', 'BE', 'BIG_ENDIAN'} (not case sensitive)", optarg);
        printUsage(argv[0]);
        exit(1);
      }
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

  // If there are unknown options
  if (optind < argc) {
    _DEATH("[Config] Invalid input");
  }

  if (!validate()) {
    printUsage(argv[0]);
    exit(1);
  }
};


bool Config::validate() const {
  if (hex_string_.empty() && filepath_.empty()) {
    _ERROR("Missing argument!");
    return false;
  }

  if (!hex_string_.empty() && !filepath_.empty()) {
    _ERROR("'-s|--hex-string' and '-f|--filepath' are mutually exclusive options!");
    return false;
  }

  return true;
}


void Config::print() const {
  std::string interpretations_str;
  if (interpretations_.empty()) {
    interpretations_str = "All";
  } else {
    for (Interpretation ipt: interpretations_) {
      if (!interpretations_str.empty())
        interpretations_str += ",";

      interpretations_str += Interpretation::interpretationToCstr(ipt);
    }
  }

  printf("----------------------------------------------------\n");
  printf("Configuration: hex-string: %s\n"
         "               filepath: %s\n"
         "               byte-order: %s\n"
         "               padding: %lu\n"
         "               interpretations: %s\n"
         "               offset: %lu\n",
         hex_string_.c_str(), filepath_.c_str(), ByteOrder::byteOrderToCstr(byte_order_), padding_, interpretations_str.c_str(), offset_);
  printf("----------------------------------------------------\n\n");
}

} //namespace scx
