#include "utils.h"
#include <netinet/in.h>
#include <stdint.h>

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

int8_t safe_write(void* data, size_t length, int sockfd) {
    size_t written_bytes = 0;
    int res;
    while (written_bytes < length) {
        errno = 0;
        res = write(sockfd, ((uint8_t*)data + written_bytes), length - written_bytes);
        if (res < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }
        if (res == 0) {
            return -1;
        }
        written_bytes += (size_t)res; 
    }
    return 0;
}

int8_t safe_read(void* data, size_t length, int sockfd) {
    size_t read_bytes = 0;
    int res = 0;
    while (read_bytes < length) {
        errno = 0;
        res = read(sockfd, (uint8_t*)data + read_bytes, length - read_bytes);
        if (res < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }
        if (res == 0) {
            return -1;
        }
        read_bytes += (size_t)res;
    }
    return 0;
}

int8_t send_message(query_header_t* header, void* msg_data, size_t data_length, int sockfd) {
    if (validate_header(header) == -1) {
        return -1;
    }
    if (safe_write(header, sizeof(query_header_t), sockfd) == -1) {
        return -1;
    }
    if (header->query_type[0] == REQUEST) { //Request to server
        if (header->query_type[3] == SQRT_MESSAGE) {
            uint64_t send_double = get_double_bigendian(*(double*)msg_data);
            if (safe_write(&send_double, sizeof(uint64_t), sockfd) == -1) {
                return -1;
            }
        }
        // Sending Request for date doesn't require additional data
    } else if (header->query_type[1] == RESPONSE) { //Response to client
        if (header->query_type[3] == SQRT_MESSAGE) {
            uint64_t send_double = get_double_bigendian(*(double*)msg_data);
            if (safe_write(&send_double, sizeof(uint64_t), sockfd) == -1) {
                return -1;
            }
        } else {
            // todo: Send date
        }
    } else { // unknown operation
        return -1;
    }
    return 0;
}

int8_t read_message(query_header_t* header, void** msg, size_t* data_length, int sockfd) {
    if (safe_read(header, sizeof(query_header_t), sockfd) == -1) {
        return -1;
    } 
    if (validate_header(header) == -1) {
        return -1;
    }
    if (header->query_type[3] == SQRT_MESSAGE) { // If header indicates SQRT message.
        double* double_val = malloc(sizeof(double));
        if (double_val == NULL) {
            return -1;
        }
        uint64_t double_read;
        if (safe_read(&double_read, sizeof(double_read), sockfd) == -1) {
            free(double_val);
            return -1;
        }
        *double_val = get_double_little_endian(double_read);
        *msg = double_val;
    } else if (header->query_type[0] == RESPONSE){ //If header indicates DATE response.
        uint32_t size = 0;
        if (safe_read(&size, sizeof(uint32_t), sockfd) == -1) {
            return -1;
        }
        size = ntohl(size);
        char* date = malloc(sizeof(char) * size);
        if (date == NULL) {
            return -1;
        }
        if (safe_read(date, size, sockfd) == -1) {
            free(date);
            return -1;
        }
        *msg = date;
    } else { // If header indicates DATE request.
        *msg = NULL;
    }

    return 0;
}

int8_t validate_header(query_header_t* header) {
    if ((header->query_type[0] != REQUEST) && (header->query_type[0] != RESPONSE)) {
        return -1;
    }
    if ((header->query_type[3] != SQRT_MESSAGE) && (header->query_type[3] != DATE_MESSAGE)) {
        return -1;
    }
    return 0;
}

// porty na bigendian
// zrobić plik config
// sprawdzać czy jest big czy litlle endian przy zamianie na little