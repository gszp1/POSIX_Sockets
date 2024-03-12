#ifndef UTILS.H
#define UTILS.H

#include <stdio.h>
#include <sys/socket.h>
#include <stdint.h>

typedef struct {
    uint8_t query_type[4];
    uint32_t request_id;
} query_identificator_t;


#endif //uitls.h