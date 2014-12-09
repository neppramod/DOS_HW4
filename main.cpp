#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <pthread.h>



#include "constants.h"
#include "server.h"
#include "communication.h"
#include "neighbors.h"
#include "client.h"

using namespace std;

void *server_handler(void *conn);
void *client_handler(void *portintvr);
void server_thread_handler(void *connection);

int node_index = 0;     // Keep track of node
neighbors neighbors_list[BUFFER_SIZE];
int socketdesc;         // Socket descriptor



int main(int argc, char *argv[])
{

    int connection;        // Return value from accept()

    int upper_bound_portnum;    //upper bound port numbers from file
    char portnum[BUFFER_SIZE];  //port number

    ifstream port_file;       // File to read port number from file
    ifstream neighbors_list_input_file;  // File to read server list (neighbors)


    Communication *communication = Communication::commucationInstance();

    pthread_t *client_thread;       //Thread for creating its own client
    pthread_t *server_thread;

    port_file.open(PORT_FILE_NAME);
    while(port_file>>portnum>>upper_bound_portnum);

    int portint = communication->create_server_connection(&socketdesc, portnum, upper_bound_portnum);



    neighbors_list_input_file.open(NEIGHBORS_FILE_LIST_NAME);

    if(neighbors_list_input_file.is_open()){

        char neighbor_host[BUFFER_SIZE];
        char neighbor_port[BUFFER_SIZE];

       //node_index=0;

        while(neighbors_list_input_file>>neighbor_host>>neighbor_port)
        {
            strcpy(neighbors_list[node_index].host_name,neighbor_host);
            strcpy(neighbors_list[node_index].host_port,neighbor_port);

            node_index++;
        }

    } else {
            cout<<"Server: Could not open neighbors" << endl;
    }


    // If there is a command-line parameter start the client
    if (argc > 1) {

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
    }


    // Server loop to handle multiple clients
    server_thread_handler(&connection);

    // Close the connection
    close(connection);
    return 0;
}

void server_thread_handler(void *connection) {

    pthread_t *new_client_threads;        // Thread for serving multiple clients
    Communication *communication = Communication::commucationInstance();



    for(;;)
    {    cout << "Waiting for connection" << endl;
        connection = (int *)(communication->client_connection_accept((int *)&socketdesc));

        if(connection > 0){

            // Create a new thread
            new_client_threads = (pthread_t *) malloc(sizeof(pthread_t));

            if ( pthread_create
                    (new_client_threads, NULL, &server_handler, (void *) &connection) < 0)
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
}


void *server_handler(void *conn)
{

    Server server;                          // Few utilities of server
    //server.start(conn);
    server.start(conn,neighbors_list,node_index);
}

void *client_handler(void *portintvr)
{
    int portint = *((int *)portintvr);
    Client client;
    client.start(portint);
}
