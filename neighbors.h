#ifndef NEIGHBORS_H
#define NEIGHBORS_H

#include "constants.h"

typedef struct{
    char host_name[BUFFER_SIZE];    //neighbor host name
    char host_port[BUFFER_SIZE];    // neighbor port number
} neighbors;

#endif