#include "server.h"

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
#include "client.h"
#include "constants.h"
#include "neighbors.h"
#include "utility.h"
#include "neighbors.h"

using namespace std;

Server::Server()
{

}

void Server::start(void *conn, neighbors *neighbors_list, int node_index)
{
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
            } else if (strcmp(serviceRequestMessage->requestString, "lookup") == 0) { // lookup file

                char complete_file_name[BUFFER_SIZE];
                strcat(complete_file_name, FILE_SEARCH_DIRECTORY);
                strcat(complete_file_name, "/");
                strcat(complete_file_name, serviceRequestMessage->payload);

                serviceRequestMessage->requestType = SERVER_RESPONSE_REQUEST;


                FILE *search_file;

                if (search_file = fopen(complete_file_name, "r")) { // file exists
                    char file_buffer[1024];
                    char file_conent[1024];
                    while (fgets(file_buffer, sizeof file_buffer, search_file) != NULL) {
                        strcat(file_conent, file_buffer);
                    }

                    cout << "Found " << serviceRequestMessage->payload << endl;
                    cout << "File conent: " << file_conent << endl;
                    strcpy(serviceRequestMessage->requestString, "found");
                    strcpy(serviceRequestMessage->payload, file_conent);

                    fclose(search_file);
                }  else { // file does not exist
                    strcpy(serviceRequestMessage->requestString, "not found");
                    cout << "File does not exist" << endl;
                }

                write(connection, (char *) serviceRequestMessage, sizeof(serviceRequest));

            }
        } else if (requestType == CLIENT_PAYLOAD_SHARE) {
            cout << "Server: (payload) : " << serviceRequestMessage->payload << endl;

            update_list(serviceRequestMessage->payload, neighbors_list, node_index);

        } else if (requestType == CLIENT_EXIT_REQUEST) {
            close(connection);
        }


    } // while exits
}


void Server::update_list(char *payload, neighbors *neighbors_list, int node_index)
{
    char **tokens;
    tokens = utility.str_split(payload, ';');
    Utility utility;

    if (tokens)
    {
        int i, j;
        char save_tokens[81];
        char new_neighbors[1024];

        for (i = 1; *(tokens + i); i+=2)
        {
            strcpy(save_tokens,"");
            strcat(save_tokens, *(tokens + i));
            strcat(save_tokens, " ");
            strcat(save_tokens, *(tokens + (i+1)));

            int found = 0;
            for (j = 0; j < node_index; j++) {
                char neighbor_address_line[81];
                strcpy(neighbor_address_line,"");
                strcat(neighbor_address_line, neighbors_list[j].host_name);
                strcat(neighbor_address_line, " ");
                strcat(neighbor_address_line, neighbors_list[j].host_port);


                if (strcmp(neighbor_address_line, save_tokens) == 0) {
                    cout << "Server: " << neighbor_address_line << " exists" << endl;
                    found = 1;
                    break;
                }
            }

            if (!found) {
                strcat(new_neighbors, save_tokens);
                strcat(new_neighbors, "\n");
            }
        }

        cout << "New neighbors " << new_neighbors << endl;
        cout<< "Entire List " << endl;
        for (j = 0; j < node_index; j++) {
            cout << neighbors_list[j].host_name << " " << neighbors_list[j].host_port << endl;
        }
        cout << new_neighbors;

        utility.saveFile(NEIGHBORS_LIST_FILE_NAME, new_neighbors);

    }
}
