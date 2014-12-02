#ifndef CLIENT_H
#define CLIENT_H

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
     Communication *communication = Communication::commucationInstance();           // Communication class for doing the communication
     int sockdesc;                           // Socket number (*not* the port number)
     int connection;     // Server connection value
     serviceRequest serviceRequestMessage; // Record for data
     int value;                          // Return value from read()

public:
    Client();
    void start(int srv_port_num);  // Start the client
    void createSocketDescriptor();  // Create socket descriptor
};

#endif // CLIENT_H
