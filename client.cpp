#include "client.h"
#include "server.h"
#include "neighbors.h"
#include "communication.h"
#include "logger.h"
#include "constants.h"
#include <iostream>

#define PAYLOAD_LIMIT 3

using namespace std;

Client::Client() {

}

// Create socket descriptor
void Client::createSocketDescriptor()
{
    // create the socket descriptor first
    communication->create_socket_descriptor(&sockdesc);
}

// Display Menu list

void Client::showMenu()
{
    int menu=0;

    cout << "\nSelect from following options" << endl << endl;
    cout << ++menu << ".Display Neighbors" << endl;
    cout << ++menu << ".Choose a Neighbor" << endl;
    cout << ++menu << ".Push the top three neighbors to the neighbor selected in 2." << endl;
    cout << ++menu << ".Search for a file on neighbor selected in 2." << endl;
    cout << ++menu << ".Search for a file on recursively stating at neighbor selected in 2." << endl;
    cout << ++menu << ".Display active nodes in the system" << endl;
    cout << ++menu << ".Quit" << endl << endl;
    cout << "option > ";
}

void Client::selectMenuOption()
{
    int menuOption = 0;

    cin >> menuOption;

    switch(menuOption) {
    case 1:
        displayNeighbors();
        break;

    case 2:
        chooseANeighbor();
        break;

    case 3:
        pushTopThreeNeighbors();
        break;

    case 4:
        searchForAFileInNeighbor();
        break;

    case 5:
        searchForAFileRecursively();
        break;

    case 6:
        displayActiveNodesInTheSystem();
        break;

    case 7:         // Quit
        cout << "Exiting .." << endl;
        exit(0);
        break;
    }


}

// Start the client. portint is this server's used port
void Client::start(int srv_port_num) {

    ofstream neighbors_list_output_file; // File to write server list (neighbors)
    string clientquery;                 // Client request
    char clientquerychar[BUFFER_SIZE];
    string node_ip;                     // IP address of the selected node
    string node_port;                   // Port number of the selected node
    int option;                         // For select node

    while (true) {
        showMenu();
        selectMenuOption();
    }
}


// Display neighbors from neighbors file
void Client::displayNeighbors()
{
    if (node_index < 1) {
        cout << "Neighbors empty. Please create a neighbors file first";
    } else {
        for (int i = 0; i < node_index; i++) {
            cout << (i + 1)
                 << ". "
                 << neighbors_list[i].host_name
                 << " "
                 << neighbors_list[i].host_port
                 << endl;
        }
    }
}

int Client::connectToANode(char *hostname, char *port)
{
    createSocketDescriptor();
    int conn = -1;

    conn = communication->create_client_connection(&sockdesc,hostname,port);

    return conn;
}

// Select one neighbor for further communication
void Client::chooseANeighbor()
{
    int selected_node_id = -1;
    cout << "Node Id: ";
    cin >> selected_node_id;

    if ((selected_node_id > node_index) && selected_node_id <= 0) {
        cout << "Invalid id" << endl;
    } else {

        strcpy(selected_neighbor.host_name,
                neighbors_list[selected_node_id - 1].host_name);

        strcpy(selected_neighbor.host_port,
                neighbors_list[selected_node_id - 1].host_port);

         // If a node was selected properly, we have to establish the connection
         // with that node
        if (connectToANode(selected_neighbor.host_name,
                           selected_neighbor.host_port) == 0) {
            Logger::log("Connection established with %s:%s\n", selected_neighbor.host_name,
                        selected_neighbor.host_port);
        } else {
            Logger::log("Could not connect to %s:%s\n", selected_neighbor.host_name,
                        selected_neighbor.host_port);
        }

    }

    //Logger::log("Hostname: %s, Port: %s\n", selected_neighbor.host_name, selected_neighbor.host_port);
}

// Push top three neighbors from neighbor list to node selected in chooseANeighbor method
void Client::pushTopThreeNeighbors()
{
     if(selected_neighbor.host_name != NULL){
         Logger::log("Connection successful connected");

         serviceRequestMessage.requestType = CLIENT_PAYLOAD_SHARE;
         strcpy(serviceRequestMessage.payload, createPayload());

         write(sockdesc, (char *)&serviceRequestMessage, sizeof(serviceRequest));
     }
}

// Search for a file in the node selected in chooseANeighbor method
void Client::searchForAFileInNeighbor()
{
    char filename[BUFFER_SIZE];
    cout << "Enter file name:";
    cin >> filename;

    if(selected_neighbor.host_name != NULL) {
        // Ping the server with request type 2
        serviceRequestMessage.requestType = CLIENT_QUERY_REQUEST;
        strcpy(serviceRequestMessage.payload, filename);
        strcpy(serviceRequestMessage.requestString, "lookup");
        write(sockdesc, (char *)&serviceRequestMessage, sizeof(serviceRequest));

        value = read(sockdesc, (char *) &serviceRequestMessage, sizeof(serviceRequest));
        cout << "Client: server sent: " << serviceRequestMessage.requestString << endl;
        if(strcmp(serviceRequestMessage.requestString, "found") == 0) {
            cout << "Client: File content: " << serviceRequestMessage.payload << endl;
        }
    }
}

// Search for a file recursively from node selected in chooseANeighbor method
void Client::searchForAFileRecursively()
{
    char filename[BUFFER_SIZE];
    cout << "Enter file name:";
    cin >> filename;

    int connect = -1;

    for (int i = 0; i < node_index; i++) {
        connect = connectToANode(neighbors_list[i].host_name,
                               neighbors_list[i].host_port);
        strcpy(serviceRequestMessage.requestString, "recursivelookup");
                    strcpy(serviceRequestMessage.payload, filename);
                    serviceRequestMessage.requestType = CLIENT_QUERY_REQUEST;

        if (connect == 0) {
            write(sockdesc, (char *)&serviceRequestMessage, sizeof(serviceRequest));
            cout << "Sending first node" << endl;
        }
    }

}

// Display active nodes in the system
void Client::displayActiveNodesInTheSystem()
{

}

// Create payload from neighbors
char *Client::createPayload()
{
    char payload[BUFFER_SIZE];

    sprintf(payload, "%d;", PAYLOAD_LIMIT);

    for (int i = 0; i < PAYLOAD_LIMIT; i++) {
        strcat(payload, neighbors_list[i].host_name);
        strcat(payload, ";");
        strcat(payload, neighbors_list[i].host_port);
        strcat(payload, ";");
    }

    payload[strlen(payload) - 1] = '\0';

    Logger::log("\nPayload: %s\n", payload);

    return payload;
}


