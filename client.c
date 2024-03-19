#include "utils.h"

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

    // Set request data
    query_header_t header;
    header.request_id = htonl(1);
    header.query_type[0] = 1;
    header.query_type[1] = 0;
    header.query_type[2] = 0;
    header.query_type[3] = 1;


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

    // Send data to server (temporary)
    char ch = 'a';
    write(sockfd, &header, sizeof(query_header_t)); 

    // Read server response (temporary)
    read(sockfd, &header, 8);
    printf("C: %u%u%u%u%u\n", header.query_type[0], header.query_type[1], header.query_type[2], header.query_type[3], ntohl(header.request_id));

    // Free resources and exit with code 0
    close(sockfd);
    return 0;
}