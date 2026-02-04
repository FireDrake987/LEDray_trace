#include <cstdint>
#pragma pack(push, 1) //Ensure no padding between fields

struct BGRPixel {
    uint8_t b;
    uint8_t g;
    uint8_t r;
};

#pragma pack(pop)
