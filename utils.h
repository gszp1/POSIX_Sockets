#ifndef UTILS.H
#define UTILS.H

//// Includes ////

#include <stdio.h>
#include <sys/socket.h>
#include <stdint.h>

//// macros definitions ////

#define BITS_PER_BYTE 8

//// types definitions ////

typedef struct {
    uint8_t query_type[4];
    uint32_t request_id;
} query_identifier_t;

//// functions declarations ////

uint8_t check_endianess()

uint32_t get_uint_bigendian(uint32_t val);

uint64_t get_double_bigendian(double val);

uint32_t get_uint_little_endian(uint32_t val);

double get_double_little_endian(uint64_t val);

#endif //uitls.h