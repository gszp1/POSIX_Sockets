#include "utils.h"
#include <stdlib.h>

int main(int argc, char* argv[]) {
    // Check passed arguments
    if (argc < 3) {
        printf("Not enough parameters provided.\n Required: IP address, port number.\n");
        return 1;
    }
    errno = 0;
    uint64_t port = strtoul(argv[1], NULL, 10);
    if ((errno == ERANGE) || (port == 0) || (port > UINT16_MAX)) {
        printf ("Second argument is not a valid port number.");
        return 2;
    }

    // Create client socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Failed to create user socket.");
        return 3;
    }
    struct sockaddr_in server_address;
    server_address.sin_port = htons((uint16_t)port);
    server_address.sin_family = AF_INET;
    inet_aton(argv[1], &server_address.sin_addr);
    
    // Connect with server


    
    
}