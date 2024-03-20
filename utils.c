#include "utils.h"

// Wrapper for parsing host double to network double
uint64_t htond(double host_double) {
    if (check_endianess() == 0) { //if host machine is big endian
        uint64_t net_double;
        memcpy(&net_double, &host_double, sizeof(net_double));
        return net_double;
    }
    return get_double_bigendian(host_double);
}

// Wrapper for parsing network double to host double
double ntohd(uint64_t net_double) {
    if (check_endianess() == 0) { //if host machine is big endian
        double host_double;
        memcpy(&host_double, &net_double, sizeof(host_double));
        return host_double;
    }
    return get_double_little_endian(net_double);
}

// Converts little endian double to big endian uint64_t
uint64_t get_double_bigendian(double val) {
    uint8_t res[8];
    memcpy (res, &val, sizeof(double));
    uint8_t result_arr[8];
    for (int i = 0; i < 8; ++i) {
        result_arr[i] = res[7 - i];
    }
    uint64_t result;
    memcpy(&result, result_arr, sizeof(result));
    return result;
}

// Converts big endian uint64_t to little endian double
double get_double_little_endian(uint64_t val) {
    uint8_t res[8];
    memcpy(res, &val, sizeof(uint64_t));
    uint8_t result_arr[8];
    for (int i = 0; i < 8; ++i) {
        result_arr[7 - i] = res[i];
    }
    double result;
    memcpy(&result, result_arr, sizeof(result));
    return result;
}

// Checks machine endianess, 0 - big endian, 1 - little endian
uint8_t check_endianess() {
    int i = 1;
    if (*((char*) & i) == 1) {
        return 1;
    }
    return 0;
}

// Sends message to socket
int8_t send_message(query_header_t* header, void* msg_data, size_t data_length, int sockfd) {
    if (validate_header(header) == -1) {
        return -1;
    }
    if (safe_write(header, sizeof(query_header_t), sockfd) == -1) {
        return -1;
    }
    if (header->query_type[3] == SQRT_MESSAGE) { // Request/Response for sqrt
        double* send_double_ptr = (double*)msg_data;
        uint64_t send_double = htond(*send_double_ptr);
        if (safe_write(&send_double, sizeof(send_double), sockfd) == -1) {
            return -1;
        }
    }  else if (header->query_type[0] == RESPONSE) {
        time_t t;
        time(&t);
        char* date = ctime(&t);
        *(date + strlen(date) - 1) = '\0'; // set for strlen, it is not being sent
        uint32_t size = strlen(date);
        uint32_t transfer_size = htonl(size);
        if (safe_write(&transfer_size, sizeof(transfer_size), sockfd) == -1) {
            return -1;
        }
        if (safe_write(date, size * sizeof(char), sockfd) == -1) {
            return -1;
        }
    } else {
        return 0;
    }
    return 0;
}

// Writes data to sockfd socket safely (re-writes when not all bytes were sent)
int8_t safe_write(void* data, size_t length, int sockfd) {
    size_t written_bytes = 0;
    int res = 0;
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

// Reads message from socket
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
        *double_val = ntohd(double_read);
        *msg = double_val;
        *data_length = 0; // 0 means that read value is a double
    } else if (header->query_type[0] == RESPONSE){ //If header indicates DATE response.
        uint32_t size = 0;
        if (safe_read(&size, sizeof(uint32_t), sockfd) == -1) {
            return -1;
        }
        size = ntohl(size);
        char* date = malloc(sizeof(char) * (size + 1));
        if (date == NULL) {
            return -1;
        }
        if (safe_read(date, size, sockfd) == -1) {
            free(date);
            return -1;
        }
        *(date + size) = '\0';
        *msg = date;
        *data_length = size;
    } else { // If header indicates DATE request.
        *msg = NULL;
    }

    return 0;
}

// Reads data from sockfd socket safely (re-reads when not all bytes were read)
int8_t safe_read(void* data, size_t length, int sockfd) {
    size_t read_bytes = 0;
    int res = 0;
    while (read_bytes < length) {
        errno = 0;
        res = read(sockfd, ((uint8_t*)data + read_bytes), length - read_bytes);
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

// Checks if header is correctly set
int8_t validate_header(query_header_t* header) {
    if ((header->query_type[0] != REQUEST) && (header->query_type[0] != RESPONSE)) {
        return -1;
    }
    if ((header->query_type[3] != SQRT_MESSAGE) && (header->query_type[3] != DATE_MESSAGE)) {
        return -1;
    }
    return 0;
}