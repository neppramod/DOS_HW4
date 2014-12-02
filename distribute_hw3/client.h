#ifndef CLIENT_H
#define CLIENT_H

#include "client.h"
#include "communication.h"
#include "message.h"
#include "constants.h"
#include "neighbors.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>

class Client
{
private:
    int sockdesc;                           // Socket number (*not* the port number)
    char hostname[BUFFER_SIZE];         // Server name
    char portnum[BUFFER_SIZE];          // Server port number (note: char* not int)
    int value;                          // Return value from read()
    serviceRequest serviceRequestMessage; // Record for data
    Communication *communication;           // Communication class for doing the communication

    //char *nodes[MAXNODES];  // Keeps a table of neighbors
    neighbors neighbors_list[BUFFER_SIZE];

    int node_index;     // Keep track of node

    int connection;     // Server connection value

    void initializeNeighboNodes();  // Read list from neighbors file and put it in nodes
    void createSocketDescriptor();  // Create socket descriptor
public:
    Client();
    void start(int srv_port_num);  // Start the client
};

#endif // CLIENT_H
