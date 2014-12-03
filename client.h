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
     neighbors selected_neighbor;    // This is the selected neighbor from question number 2 (e.g Bob)

     char *createPayload();

public:
    Client();
    void start(int srv_port_num);  // Start the client
    void createSocketDescriptor();  // Create socket descriptor
    void showMenu();               // Display menu
    void selectMenuOption();         // Select one from the showMenu()
    void displayNeighbors();        // Display neighbors from neighbors file
    void chooseANeighbor();          // Select one neighbor for further communication
    void pushTopThreeNeighbors();   // Push top three neighbors from neighbor list to node selected in chooseANeighbor method
    void searchForAFileInNeighbor();    // Search for a file in the node selected in chooseANeighbor method
    void searchForAFileRecursively();   // Search for a file recursively from node selected in chooseANeighbor method
    void displayActiveNodesInTheSystem();   // Display active nodes in the system
    int connectToANode(char *hostname, char *port);  // Connect selected node

};

#endif // CLIENT_H
