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
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
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
        // accepting connections
        int client_socket_fd = accept(server_socket_fd,
            (struct sockaddr *)(&client_sockaddr),
        &client_len
            );
        if (client_socket_fd == -1) {
            continue;
        }
        // display used socket and IPv4 address
        printf("S: Client connected from IPv4:port: %s:%u\n",
                inet_ntoa(client_sockaddr.sin_addr),
                ntohs(client_sockaddr.sin_port)
                );
        pid_t fork_res = fork();
        if (fork_res == 0) { // Child process
            while (1) { // Execute until client disconnects
                query_header_t header;
                void* read_msg = NULL;
                size_t data_length = 0;
                double val = 0;
                int op_res = read_message(&header, &read_msg, &data_length, client_socket_fd);
                if (op_res == -1) {
                   break;
                }
                header.query_type[0] = RESPONSE;
                if (header.query_type[3] == SQRT_MESSAGE) {
                    val = *((double*)read_msg);
                    printf("S: Handling SQRT query id: %u, value: %f, client IPv4:port: %s:%u\n",
                            ntohl(header.message_id),
                            val,
                            inet_ntoa(client_sockaddr.sin_addr),
                            ntohs(client_sockaddr.sin_port)
                    );
                    val = sqrt(val);
                } else {
                        printf("S: Handling DATE query id: %u, client IPv4:port: %s:%u\n",
                        ntohl(header.message_id),
                        inet_ntoa(client_sockaddr.sin_addr),
                        ntohs(client_sockaddr.sin_port)
                    );
                }
                if (read_msg != NULL) {
                    free(read_msg);
                }
                op_res = send_message(&header, &val,client_socket_fd);
                if (op_res == -1) {
                    break;
                }
            }
            close(client_socket_fd);
            return 0;
        }
        if (fork_res == -1) {
            printf("Failed to spawn process for handling new client connection.\n");
            close(client_socket_fd);
        }
    }

    // Free used resources
    close(server_socket_fd);
    // exit
    return 0;
}