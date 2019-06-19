#pragma once

#include <string>

namespace hx {

static uint64_t ntoh64(const uint64_t& input) {
    uint64_t rval;
    uint8_t* data = reinterpret_cast<uint8_t*>(&rval);

    data[0] = static_cast<uint8_t>(input >> 56);
    data[1] = static_cast<uint8_t>(input >> 48);
    data[2] = static_cast<uint8_t>(input >> 40);
    data[3] = static_cast<uint8_t>(input >> 32);
    data[4] = static_cast<uint8_t>(input >> 24);
    data[5] = static_cast<uint8_t>(input >> 16);
    data[6] = static_cast<uint8_t>(input >> 8);
    data[7] = static_cast<uint8_t>(input >> 0);

    return rval;
}

class Buffer {
public:
    Buffer();
    
    ~Buffer();

    void initFromHexString(const std::string& hex_str);

    uint64_t getUInt64() const {
        uint64_t value = *reinterpret_cast<const uint64_t*>(data_ + offset_); 
        if (network_byte_order_) {
            return ntoh64(value);
        } else {
            return value;
        }
    }

    uint32_t getUInt32() const {
       return *reinterpret_cast<const uint32_t*>(data_ + offset_); 
    }

private:
    char*       data_;
    size_t      len_;
    size_t      offset_ = 0;
    bool        network_byte_order_ = false;
};

} // namespace hx
