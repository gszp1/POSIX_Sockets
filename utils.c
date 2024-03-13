#include "utils.h"

uint32_t get_uint_bigendian(uint32_t val) {
    uint8_t res[4];
    int8_t i = 3;
    while (i >= 0) {
        res[i] = ((uint64_t)val >> (i * BITS_PER_BYTE)) % 0xff;
        --i;
    }
    return *((uint32_t*)res);
}

uint64_t get_double_bigendian(double val) {
    uint8_t res[8];
    int8_t i = 7;
    while (i >= 0) {
        res[i] = ((uint64_t)val >> (i * BITS_PER_BYTE)) % 0xff;
        --i;
    }
    return *((uint64_t*)res);
}

// porty na bigendian
// zrobić plik config 
// sprawdzać czy jest big czy litlle endian przy zamianie na little