#include "data/FileReader.h"

#include <climits>
#include <cstdio>
#include <errno.h>

#include "MainUtils.h"

namespace hx {

FileReader::FileReader(const std::string& filepath, size_t initial_offset) : file_(nullptr), file_size_(0), current_data_(nullptr), current_data_capacity_(0) {
  // Opening the file
  file_ = fopen(filepath.c_str(), /*filemode*/"rb");
  if (!file_)
    _DEATH("Could not open file '%s' (errno: %d)", filepath.c_str(), errno);

  // Seeking the end of the file before "telling" its size
  if (fseek(file_, 0, SEEK_END) != 0)
    _DEATH("Could not seek end of file '%s' (errno: %d)", filepath.c_str(), errno);

  // TODO: what if file is extremely large?
  long size = ftell(file_);
  if (size < 0)
    _DEATH("Could not ftell at end of file '%s' (errno: %d)", filepath.c_str(), errno);

  file_size_ = static_cast<size_t>(size);

  if (initial_offset > LONG_MAX)
    _DEATH("The current version of hexibit cannot handle large offsets");

  if (file_size_ <= initial_offset)
    _DEATH("File '%s' is too small to be read at offset '%zu'", filepath.c_str(), initial_offset);

  // After construction the file_ position is set to initial_offset
  if (fseek(file_, static_cast<long>(initial_offset), SEEK_SET) != 0)
    _DEATH("Could not start reading file '%s' at offset %zu (errno: %d)", filepath.c_str(), initial_offset, errno);
}


FileReader::~FileReader() {
  if (file_) {
    if (fclose(file_) != 0)
      _DEATH("Error while closing file!");
  }

  if (current_data_)
    free(current_data_);
}


void FileReader::advanceReadPtr(size_t num_bytes) {
  _ASSERT(num_bytes <= LONG_MAX); // Does not handle large files
  if (fseek(file_, static_cast<long>(num_bytes), SEEK_CUR) != 0)
    _DEATH("Could not advance file position (errno: %d)", errno);
}


size_t FileReader::getRemainingLength() const {
  const long cur_offset = ftell(file_);
  if (cur_offset < 0)
    _DEATH("ftell() failed (errno: %d)", errno);

  return file_size_ - static_cast<size_t>(cur_offset);
}


const char* FileReader::getReadPtr(size_t num_bytes) {
  if (num_bytes > getRemainingLength())
    _DEATH("Trying to read bytes beyond file size!");

  if (num_bytes > current_data_capacity_) {
    current_data_ = static_cast<char*>(realloc(current_data_, num_bytes));
    if (!current_data_)
      _DEATH("realloc() failed!");
  }

  const size_t read_len = fread(current_data_, sizeof(char), num_bytes, file_);
  if (read_len != num_bytes)
    _DEATH("Error reading data!");

  //TODO: improve that to avoid repetitive reads
  if (fseek(file_, static_cast<long>(-read_len), SEEK_CUR) != 0)
    _DEATH("Could not reset file position (errno: %d)", errno);

  return current_data_;
}

} // namespace ct
