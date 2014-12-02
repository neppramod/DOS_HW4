#include "client.h"
#include "server.h"
#include "neighbors.h"
#include "communication.h"
#include <iostream>

using namespace std;

Client::Client() {

}

// Create socket descriptor
void Client::createSocketDescriptor()
{
    // create the socket descriptor first
    communication->create_socket_descriptor(&sockdesc);
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

        cout << "0: quit " << endl;
        if( node_index < 1){
            // ask for the ip
            cout << "Client: Enter a node's ip address and port number to connect" <<endl;
            cin>> node_ip >> node_port;
        } else {
            // Show the list here
            for (int i = 0; i < node_index; i++) {
                cout << (i + 1) << ". " << neighbors_list[i].host_name << " " << neighbors_list[i].host_port << endl;
            }


            cout << "Client: Enter node number to connect" << endl;
            cout << "Client : ";
            cin >> option;

            if( option > 0  && option <= node_index){
                node_ip = neighbors_list[option-1].host_name;
                node_port = neighbors_list[option-1].host_port;

            } else if (option == 0) {
                cout << "Exiting" << endl;
                break;
            }

            else {
                cout << "Invalid optoin, Please select again" << endl;
            }

            // Connect to the server

             createSocketDescriptor();


             char *tmp_node_ip= strdup(node_ip.c_str());
             char *tmp_node_port = strdup(node_port.c_str());

             connection = communication->create_client_connection(&sockdesc, tmp_node_ip , tmp_node_port);

             if (connection == 0) {
                 // Ping the server with request type 2
                 serviceRequestMessage.requestType = CLIENT_QUERY_REQUEST;

                 strcpy(serviceRequestMessage.requestString, "ping");
                 write(sockdesc, (char *)&serviceRequestMessage, sizeof(serviceRequest));

                 value = read(sockdesc, (char *) &serviceRequestMessage, sizeof(serviceRequest));
                 cout << "Client: server sent: " << serviceRequestMessage.requestString << endl;

             }

        }

    }
}

