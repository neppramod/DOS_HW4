#ifndef  SERVER_H
#define  SERVER_H

#include "neighbors.h"

extern  neighbors neighbors_list[BUFFER_SIZE];

class Server
{

public:
    Server();
    void start(void *conn, neighbors *neighbors_list, int node_index);

};

#endif