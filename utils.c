#include "utils.h"

uint8_t check_endianess() {
    // 0 if big endian, 1 if little endian
    int i = 1;
    if (*((char*) & i) == 1) {
        return 1;
    }
    return 0;
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

double get_double_little_endian(uint64_t val) {
    uint64_t res = 0;
    int8_t i = 7;
    while (i >= 0) {
        res |= (val << (BITS_PER_BYTE * i));
        --i;
    }
    return *((double*)res);
}

// porty na bigendian
// zrobić plik config
// sprawdzać czy jest big czy litlle endian przy zamianie na little