/**
* main.cpp
* A simple tcp/ip socket-based server client
*/

// System include's
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <fstream>

// App include's
#include "communication.h"
#include "client.h"
#include "server.h"

// For threading
void *server_handler (void *sock_desc);
void *client_handler(void *sock_desc);

int node_index = 0;     // Keep track of node
neighbors neighbors_list[BUFFER_SIZE];

using namespace std;



int main()
{
    int sockdesc;            // Socket descriptor
    char portnum[BUFFER_SIZE];        // Port number
    int upper_bound_portnum; // upper bound port numbers from file
    ifstream port_file;      // File to read port number (portnums)
    ifstream neighbors_list_input_file;  // File to read server list (neighbors)
    int connection;          // Return value from accept()

    Communication communication;            // Communication class for doing the communication
    pthread_t *other_client_threads;        // Thread for serving multiple clients
    pthread_t *client_thread;               // Thread for creating its own client



    // Get the port number from file called portnums
    port_file.open(PORT_FILE_NAME);
    while (port_file >> portnum >> upper_bound_portnum);

    cout << "Server: Port " << portnum << " in use" << endl;
    int portint = communication.create_server_connection(&sockdesc, portnum, upper_bound_portnum);

    // Display a list of known neighbors
    cout << "Use following options " << endl;
    cout << "--------------------------" << endl;

    neighbors_list_input_file.open(NEIGHBORS_LIST_FILE_NAME);

    if (neighbors_list_input_file.is_open())
    {
        char neighbors_host[BUFFER_SIZE];
        char neighbors_port[BUFFER_SIZE];
        char neighbors_address[CLIENT_DOMAIN_NAME_LENGTH];
        int i = 0;


        cout << "s: query" << endl;
        cout << "0. none" << endl;
        while(neighbors_list_input_file >> neighbors_host >> neighbors_port)
        {
            cout << (i+1) << ". " << neighbors_host << " " << neighbors_port << endl;

            i++;
            strcpy(neighbors_list[node_index].host_name, neighbors_host);
            strcpy(neighbors_list[node_index].host_port, neighbors_port);
            node_index++;

        }
    }
    else
    {
        cout << "Server: Could not open neighbors" << endl;
    }

    cout << "-----------------------------" << endl;


    // Create a client thread after starting the server
    client_thread = (pthread_t *) malloc (sizeof (pthread_t));

    if (pthread_create
            (client_thread, NULL, &client_handler, (void *) &portint) < 0)
    {
        cout << "Server: Error in creating default client thread" << endl;
    }
    else
    {
        cout << "Server: Default client creation successful" << endl;
    }


    // Server loop to handle multiple clients
    for (;;)
    {
        connection = communication.client_connection_accept(&sockdesc);

        if (connection >= 0)
        {
            // Create a new thread
            other_client_threads = (pthread_t *) malloc(sizeof(pthread_t));

            if (pthread_create
                    (other_client_threads, NULL, &server_handler, (void *) &connection) < 0)
            {
                cout << "Server: Error in creating thread" << endl;
            }
            else
            {
                cout << "Server: Thread creation successful" << endl;
                continue;
            }
        }
    }

    // Close the connection
    close(connection);
    return 0;
}

void *client_handler(void *portintvr)
{
    int portint = *((int *)portintvr);
    Client client;
    client.start(portint);
}

void *server_handler(void *conn)
{
    Server server;                          // Few utilities of server
    //server.start(conn);
    server.start(conn,neighbors_list,node_index);
}


