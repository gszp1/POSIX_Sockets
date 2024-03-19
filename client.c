#include "utils.h"

int main(int argc, char* argv[]) {
    // Check passed arguments
    if (argc < 3) {
        printf("Not enough parameters provided.\n Required: IPv4 address, port number.\n");
        return 1;
    }

    // Prepare query header
    query_header_t header;
    header.message_id = htonl(1);
    header.query_type[0] = REQUEST;
    header.query_type[1] = 0;
    header.query_type[2] = 0;
    header.query_type[3] = SQRT_MESSAGE;
    double data = 4;
    uint64_t net_double = htond(data);

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

    // Send request to server
    int res = send_message(&header, &net_double, sizeof(data), sockfd);
    if (res == -1) {
        printf("Failed to send request to server.\n");
        close(sockfd);
        return 5;
    }
    // Read response from server
    void* query_result = NULL;
    query_header_t result_header;
    size_t read_size = 0;
    res = read_message(&result_header, &query_result, &read_size, sockfd);
    if (res == -1) {
        printf ("Failed to receive response from server.\n");
        close(sockfd);
        return 6;
    }

    // Close connection
    close(sockfd);

    // Display result
    if (read_size == 0) {
        double result = ntohd(*((double*)query_result));
        printf("S: Query result: %f\n", result);
    } else {
        char* date = (char*)query_result;
        for (int i = 0; i < read_size; ++i) {
            putchar(*date);
            ++date;
        }
    }
    free(query_result);

    return 0;
}
