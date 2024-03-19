#include "utils.h"
#include <stdint.h>

int main() {
    // Create server socket
    int server_socket_fd;
    server_socket_fd = socket(AF_INET, SOCK_STREAM, PROTOCOL_AUTO);
    if (server_socket_fd == -1) {
        printf("Failed to create server socket.");
        return 1;
    }
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_port = htons(9090);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY); // listen to connections on all available IPv4 addresses on machine
    server_sockaddr.sin_family = AF_INET;

    // display used socket and IPv4 address
    printf("Server started at port: %u\nIPv4 address: %s\n",
            ntohs(server_sockaddr.sin_port),
            inet_ntoa(server_sockaddr.sin_addr));

    // bind address to server socket
    int bind_res = bind(server_socket_fd,
       (struct sockaddr*)(&server_sockaddr),
        sizeof(server_sockaddr)
        );
    if (bind_res == -1) {
        printf("Failed to bind address to socket.\n");
        close(server_socket_fd);
        return 2;
    }

    // set server socket for listening to incoming connections
    if (listen(server_socket_fd, 10) == -1) {
        printf("Failed to begin connection listening.");
        close(server_socket_fd);
        return 3;
    }
    printf("S: Listening for connections.\n");

    // connections loop
    struct sockaddr_in client_sockaddr;
    socklen_t client_len;
    while (1) {
        // connection acceptance
        int client_socket_fd = accept(server_socket_fd,
            (struct sockaddr *)(&client_sockaddr),
        &client_len
            );
        if (client_socket_fd == -1) {
            continue;
        }
        // display used socket and IPv4 address
        printf("Client connected from port: %u\nIPv4 address: %s\n",
                ntohs(client_sockaddr.sin_port),
                inet_ntoa(client_sockaddr.sin_addr));
        query_header_t request_header;
        void* data = NULL;
        size_t length = 0;
        read_message(&request_header, &data, &length, client_socket_fd);
        double response_data = ntohd(*((uint64_t*)data));
        printf("%f\n", response_data);
        response_data = sqrt(response_data);
        printf("%f\n", response_data);
        request_header.query_type[0] = RESPONSE;
        uint64_t send_data = htond(response_data);
        send_message(&request_header, &send_data, sizeof(send_data), client_socket_fd);
        close(client_socket_fd);
    }

    // Free used resources
    close(server_socket_fd);
    // exit
    return 0;
}