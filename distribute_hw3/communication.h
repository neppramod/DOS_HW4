#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <cstdio>

/**
* @brief The Communication class abstracts connection methods for
* server and client
*/
class Communication
{
public:
    Communication();

    /* Create communication for client. Returns connection status */
    int create_client_connection(int *sockdesc, char *hostname, char *portnum);

    /* Create communication for server and return used port number */
    int create_server_connection(int *sockdesc, char *portnum, int upper_bound_portnum);

    /* Client: Called once, thus out from create_client_connection */
    void create_socket_descriptor(int *sockdesc);

    /* Server: Accept a client's connection and return value of connection */
    int client_connection_accept(int *sockdesc);
};

#endif // COMMUNICATION_H
