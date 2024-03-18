#ifndef UTILS_H
#define UTILS_H

//// Includes ////

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>

//// macros definitions ////

#define BITS_PER_BYTE 8
#define PROTOCOL_AUTO 0

//// types definitions ////

typedef struct {
    uint8_t query_type[4];
    uint32_t request_id;
} query_header_t;

//// functions declarations ////

uint8_t check_endianess();

uint32_t get_uint_bigendian(uint32_t val);

uint64_t get_double_bigendian(double val);

uint32_t get_uint_little_endian(uint32_t val);

double get_double_little_endian(uint64_t val);

#endif //uitls.h