#ifndef UTILS.H
#define UTILS.H

#include <stdio.h>
#include <sys/socket.h>
#include <stdint.h>

#define BITS_PER_BYTE 8

typedef struct {
    uint8_t query_type[4];
    uint32_t request_id;
} query_identifier_t;


uint32_t get_uint_bigendian(uint32_t val);

uint64_t get_double_bigendian(double val);

#endif //uitls.h