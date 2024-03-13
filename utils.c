#include "utils.h"

uint8_t check_endianess() {
    // 0 if big endian, 1 if little endian
    int i = 1;
    if (*((char*) & i) == 1) {
        return 1;
    }
    return 0;
}


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

uint32_t get_uint_little_endian(uint32_t val) {

}

double get_double_little_endian(uint64_t val) {

}

// porty na bigendian
// zrobić plik config
// sprawdzać czy jest big czy litlle endian przy zamianie na little