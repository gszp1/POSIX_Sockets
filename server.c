#include "utils.h"

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
    server_sockaddr.sin_addr.s_addr = INADDR_ANY;
    server_sockaddr.sin_family = AF_INET;
    // display used socket and IPv4 address

    // listen for incoming connection requests

    // for each request spawn process

        // handle request

        // exit

    // Free used resources

    // exit
}