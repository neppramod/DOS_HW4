#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <fstream>

// App include's
#include "message.h"
#include "communication.h"
//#include "client.h"
#include "constants.h"
#include "neighbors.h"

#include "neighbors.h"
#include "server.h"


using namespace std;

Server::Server() {
}

void Server::start(void *conn, neighbors *neighbors_list, int node_index) {

    int value;                                  // Return value from read
    serviceRequest *serviceRequestMessage;      // Service request message object
    int connection = *((int *) conn);
    int requestType = 0;  // Simple variable for serviceRequestMessage->requestType
    ifstream neighbors_list_input_file;  // File to read server list (neighbors)
    ofstream neighbors_list_output_file; // File to write server list (neighbors)


    // Wait while request type 1 (exit) is not requested from client
    while(requestType != CLIENT_EXIT_REQUEST) {
        serviceRequestMessage = (serviceRequest *) malloc(sizeof(serviceRequest));
        value = read (connection, (char *) serviceRequestMessage, sizeof (serviceRequest));
        cout << "Server: client sent: " << serviceRequestMessage->requestString << endl;

        if(value != sizeof (serviceRequest))
        {
            cout << "Server: Error, read( ) returned a bad value" << endl;
        }

        requestType = serviceRequestMessage->requestType;
        cout << "Server: Client requestType: " << requestType << endl;

        if (requestType == CLIENT_QUERY_REQUEST) {

            if (strcmp(serviceRequestMessage->requestString, "ping") == 0) { // ping
                // Change serviceRequestMessage's requestType to SERVER_RESPONSE_REQUEST (3)
                serviceRequestMessage->requestType = SERVER_RESPONSE_REQUEST;
                strcpy(serviceRequestMessage->requestString, "alive");

                write(connection, (char *) serviceRequestMessage, sizeof(serviceRequest));
              }
        } else if (requestType == CLIENT_PAYLOAD_SHARE) {
            cout << "Server: (payload) : " << serviceRequestMessage->payload << endl;

            //update_list(serviceRequestMessage->payload, neighbors_list, node_index);

        } else if (requestType == CLIENT_EXIT_REQUEST) {
            close(connection);
        }


    } // while exits
}

