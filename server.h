#ifndef  SERVER_H
#define  SERVER_H

#include "neighbors.h"
#include "utility.h"
#include "message.h"
#include "client.h"

extern  neighbors neighbors_list[BUFFER_SIZE];
extern int node_index;     // Keep track of node

class Server
{

private:
    void update_list(char *payload); // Save unique nodes from payload to neighbors
    Utility utility;
    Client client;                   // client required for visiting nodes

public:
    Server();
    void start(void *conn, neighbors *neighbors_list, int node_index);
    void update_list(char *payload, neighbors *neighbors_list, int node_index);
    char* searchFile(char *fileName);   // search for a file node
    int findVisited(char *buffer, char *host, char *port);      // find unique host and port
};

#endif
