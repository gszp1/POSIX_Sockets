#ifndef UTILS_H
#define UTILS_H

//// Included header files ////

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

//// Macro definitions ////

#define PROTOCOL_AUTO 0
#define SQRT_MESSAGE 1
#define DATE_MESSAGE 2
#define REQUEST 0
#define RESPONSE 1

//// Type definitions ////

typedef struct {
    uint8_t query_type[4]; // REQUEST/RESPONSE | 0 | 0 | SQRT_MESSAGE/DATE_MESSAGE
    uint32_t message_id;
} query_header_t;

//// Function declarations ////

// Wrapper for parsing host double to network double
uint64_t htond(double host_double);

// Wrapper for parsing network double to host double
double ntohd(uint64_t net_double);

// Converts little endian double to big endian uint64_t
uint64_t get_double_bigendian(double val);

// Converts big endian uint64_t to little endian double
double get_double_little_endian(uint64_t val);

// Checks machine endianess, 0 - big endian, 1 - little endian
uint8_t check_endianess();

// Sends message to socket
int8_t send_message(query_header_t* header, void* msg_data, int sockfd);

// Writes data to sockfd socket safely (re-writes when not all bytes were sent)
int8_t safe_write(void* data, size_t length, int sockfd);

// Reads message from socket
int8_t read_message(query_header_t* header, void** msg, size_t* data_length, int sockfd);

// Reads data from sockfd socket safely (re-reads when not all bytes were read)
int8_t safe_read(void* data, size_t length, int sockfd);

// Checks if header is correctly set
int8_t validate_header(query_header_t* header);

#endif //UTILS_h