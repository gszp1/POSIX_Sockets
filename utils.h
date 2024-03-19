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
#include <time.h>

//// macros definitions ////

#define BITS_PER_BYTE 8
#define PROTOCOL_AUTO 0
#define SQRT_MESSAGE 1
#define DATE_MESSAGE 2
#define REQUEST 0
#define RESPONSE 1

//// types definitions ////

typedef struct {
    uint8_t query_type[4]; // request[0]/response[1] | 0 | 0 | sqrt[1]/date[2]
    uint32_t message_id;
} query_header_t;

//// functions declarations ////

uint8_t check_endianess();

uint64_t get_double_bigendian(double val);

double get_double_little_endian(uint64_t val);

int8_t safe_write(void* data, size_t length, int sockfd);

int8_t safe_read(void* data, size_t length, int sockfd);

int8_t send_message(query_header_t* header, void* msg_data, size_t data_length, int sockfd);

int8_t validate_header(query_header_t* header);
#endif //uitls.h