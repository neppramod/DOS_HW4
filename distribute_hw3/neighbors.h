#ifndef NEIGHBORS_H
#define NEIGHBORS_H

#include "constants.h"

typedef struct {
    char host_name[BUFFER_SIZE]; // neighbors host name
    char host_port[BUFFER_SIZE]; // neighbors port number
} neighbors;

#endif // NEIGHBORS_H
