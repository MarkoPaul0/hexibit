#include "Buffer.h"

#include "UtilDefs.h"

namespace hx {

static int parseByteHalf(const char* p) {
    if (*p >= 'A') { // ABCDEF
        return static_cast<int>(*p - 'A');
    } else { // 0123456789
        return static_cast<int>(*p - '0');
    }        
}

Buffer::Buffer() : data_(nullptr), len_(0) {
}
    
Buffer::~Buffer() {
    if (data_)
        delete data_;
} 

void Buffer::initFromHexString(const std::string& hex_str) {
    const char* p_in = hex_str.c_str();
    len_ = hex_str.size()/2;
    data_ = new char[len_];
    char* p_out = data_;
    for (size_t i = 0; i < len_; ++i) {
        const int high_half = parseByteHalf(p_in++);
        const int low_half = parseByteHalf(p_in++);
//        _INFO("%d, %d", high_half, low_half);
        *p_out = static_cast<char>(high_half*16 + low_half);
        ++p_out;
    }
}


} // namespace ct
