#ifndef UTILS_H
#define UTILS_H

//// headers includes////

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
#include <string.h>

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

// checking machine endianess, 0 - big endian, 1 - little endian
uint8_t check_endianess();

// converts little endian double to big endian uint64_t
uint64_t get_double_bigendian(double val);

// converts big endian uint64_t to little endian double
double get_double_little_endian(uint64_t val);

// writes data to sockfd socket safely (re-writes when it is necessary)
int8_t safe_write(void* data, size_t length, int sockfd);

// reads data from sockfd socket safely (re-reads when it is necessary)
int8_t safe_read(void* data, size_t length, int sockfd);

// sends message to socket
int8_t send_message(query_header_t* header, void* msg_data, size_t data_length, int sockfd);

// checks if header is correctly set
int8_t validate_header(query_header_t* header);

// reads message from socket
int8_t read_message(query_header_t* header, void** msg, size_t* data_length, int sockfd);

#endif //UTILS_h