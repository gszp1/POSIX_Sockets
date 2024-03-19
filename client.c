#include "utils.h"
#include <stdint.h>

void read_response(void** read_result, int sockfd);

int8_t send_date_request(uint32_t msg_id, int sockfd);

// Function for sending request for sqrt result to sockfd
// 0 - success, -1 - failure
int8_t send_sqrt_request(uint32_t msg_id, int sockfd, double value);

int8_t safe_write(uint8_t* ptr, size_t len);

int main(int argc, char* argv[]) {
    // Check passed arguments
    if (argc < 3) {
        printf("Not enough parameters provided.\n Required: IPv4 address, port number.\n");
        return 1;
    }
    errno = 0;
    uint64_t port = strtoul(argv[2], NULL, 10);
    if ((errno == ERANGE) || (port == 0) || (port > UINT16_MAX)) {
        printf ("Second argument is not a valid port number.\n");
        return 2;
    }

    // Create client socket
    int sockfd = socket(AF_INET, SOCK_STREAM, PROTOCOL_AUTO);
    if (sockfd == -1) {
        printf("Failed to create user socket.\n");
        return 3;
    }
    struct sockaddr_in server_address;
    server_address.sin_port = htons((uint16_t)port);
    server_address.sin_family = AF_INET;
    inet_aton(argv[1], &server_address.sin_addr);
    socklen_t socklen = sizeof (server_address);
    
    // Connect with server
    int conn_res = connect(sockfd, (struct sockaddr*)(&server_address), socklen);
    if (conn_res != 0) {
        printf("Failed to connect with server.\n");
        close(sockfd);
        return 4;
    }   

    close(sockfd);
    return 0;
}

int8_t send_date_request(uint32_t msg_id, int sockfd) {
    // Define header
    query_header_t header;
    header.message_id = htonl(msg_id);
    header.query_type[0] = REQUEST;
    header.query_type[1] = 0;
    header.query_type[2] = 0;
    header.query_type[3] = DATE_MESSAGE;

    // Send date packet
    int res = write(sockfd, &header, sizeof(header));
    if (res == -1) {
        return -1;
    }

    return 0;
}

int8_t send_sqrt_request(uint32_t msg_id, int sockfd, double value) {
    // Define header
    query_header_t header;
    header.message_id = htonl(msg_id);
    header.query_type[0] = REQUEST;
    header.query_type[1] = 0;
    header.query_type[2] = 0;
    header.query_type[3] = SQRT_MESSAGE;

    // Send header packet
    int res = write(sockfd, &header, sizeof(header));
    if (res == -1) {
        return -1;
    }
    
    // Send double packet
    uint64_t write_double = get_double_bigendian(value);
    res = write(sockfd, &write_double, sizeof(write_double));
    if (res == -1) {
        return -1;
    }

    return 0;
}