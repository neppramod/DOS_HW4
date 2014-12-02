#ifndef SERVER_H
#define SERVER_H

#include "neighbors.h"
#include "utility.h"

class Server
{
private:
    Utility utility;
public:
    Server();
    void start(void *conn, neighbors *neighbors_list, int node_index);
    void update_list(char *payload, neighbors *neighbors_list, int node_index);
};

#endif // SERVER_H