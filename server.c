#include "utils.h"
#include <netinet/in.h>

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
    
    // listen for incoming connection requests

    // for each request spawn process

        // handle request

        // exit

    // Free used resources

    // exit
}