#include "parse/Interpretation.h"

#include <cstdint>
#include <errno.h>

#include "MainUtils.h"

namespace hx {

static constexpr size_t CHAR_ARRAY_OFFSET = 10;
static constexpr size_t SKIPPED_OFFSET    = 8;

static size_t parseInterpretationLength(const std::string& interpretation_str, size_t offset) {
  std::string len_str = interpretation_str.substr(offset);
  char* endptr;
  errno = 0;
  unsigned long len = strtoul(len_str.c_str(), &endptr, /*base*/10);
  if (errno != 0 || len_str.c_str() == endptr)
    _DEATH("Malformated interpretation %s", interpretation_str.c_str());

  return static_cast<size_t>(len);
}


Interpretation::Interpretation() : type_(UINT8), size_(0) {
}


const char* Interpretation::interpretationToCstr(Interpretation itp) {
  switch (itp.type_) {
    case Interpretation::UINT8:       return "UINT8";
    case Interpretation::UINT16:      return "UINT16";
    case Interpretation::UINT32:      return "UINT32";
    case Interpretation::UINT64:      return "UINT64";
    case Interpretation::INT8:        return "INT8";
    case Interpretation::INT16:       return "INT16";
    case Interpretation::INT32:       return "INT32";
    case Interpretation::INT64:       return "INT64";
    case Interpretation::DOUBLE:      return "DOUBLE";
    case Interpretation::BOOL:        return "BOOL";
    case Interpretation::IPV4:        return "IPV4";
    case Interpretation::SKIPPED:     return "SKIPPED";
    case Interpretation::CHAR_ARRAY:  return "CHAR_ARRAY";
    default:                          _DEATH("Unknown intepretation"); //Add %d
  }
}


bool Interpretation::strToInterpretation(const std::string& interpretation_str, Interpretation* interpretation_out) {
  bool valid = true;

  if (interpretation_str == "UINT8") {
    interpretation_out->type_ = Interpretation::UINT8;
    interpretation_out->size_ = sizeof(uint8_t);
  } else if (interpretation_str == "UINT16") {
    interpretation_out->type_ = Interpretation::UINT16;
    interpretation_out->size_ = sizeof(uint16_t);
  } else if (interpretation_str == "UINT32") {
    interpretation_out->type_ = Interpretation::UINT32;
    interpretation_out->size_ = sizeof(uint32_t);
  } else if (interpretation_str == "UINT64") {
    interpretation_out->type_ = Interpretation::UINT64;
    interpretation_out->size_ = sizeof(uint64_t);
  } else if (interpretation_str == "INT8") {
    interpretation_out->type_ = Interpretation::INT8;
    interpretation_out->size_ = sizeof(int8_t);
  } else if (interpretation_str == "INT16") {
    interpretation_out->type_ = Interpretation::INT16;
    interpretation_out->size_ = sizeof(int16_t);
  } else if (interpretation_str == "INT32") {
    interpretation_out->type_ = Interpretation::INT32;
    interpretation_out->size_ = sizeof(int32_t);
  } else if (interpretation_str == "INT64") {
    interpretation_out->type_ = Interpretation::INT64;
    interpretation_out->size_ = sizeof(int64_t);
  } else if (interpretation_str == "DOUBLE") {
    interpretation_out->type_ = Interpretation::DOUBLE;
    interpretation_out->size_ = sizeof(double);
  } else if (interpretation_str == "BOOL") {
    interpretation_out->type_ = Interpretation::BOOL;
    interpretation_out->size_ = sizeof(bool);
  } else if (interpretation_str.size() >= CHAR_ARRAY_OFFSET && interpretation_str.substr(0, CHAR_ARRAY_OFFSET) == "CHAR_ARRAY") {
    interpretation_out->type_ = Interpretation::CHAR_ARRAY;
    interpretation_out->size_ = 0;
    if (interpretation_str.size() > CHAR_ARRAY_OFFSET) { // Was provided with a length, e.g. "CHAR_ARRAY_14"
      interpretation_out->size_ = parseInterpretationLength(interpretation_str, CHAR_ARRAY_OFFSET + 1);
      if (interpretation_out->size_ <= 0)
        _DEATH("Interpretation '%s' cannot have a length of 0", interpretation_str.c_str());
    }
  } else if (interpretation_str.size() > SKIPPED_OFFSET && interpretation_str.substr(0, SKIPPED_OFFSET) == "SKIPPED_") {
    interpretation_out->type_ = Interpretation::SKIPPED;
    interpretation_out->size_ = parseInterpretationLength(interpretation_str, SKIPPED_OFFSET);
    if (interpretation_out->size_ <= 0)
      _DEATH("Interpretation '%s' cannot have a length of 0", interpretation_str.c_str());
  } else if (interpretation_str == "IPV4") {
    interpretation_out->type_ = Interpretation::IPV4;
    interpretation_out->size_ = 4;
  } else {
    valid = false;
  }

  return valid;
}


} // namespace hx


