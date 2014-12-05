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
#include "constants.h"
#include "neighbors.h"
#include "server.h"
#include "utility.h"
#include "logger.h"


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

              } else if (strcmp(serviceRequestMessage->requestString, "lookup") == 0) { // lookup file


                char complete_file_name[BUFFER_SIZE];
                strcat(complete_file_name, FILE_SEARCH_DIRECTORY);
                strcat(complete_file_name, "/");
                strcat(complete_file_name, serviceRequestMessage->payload);

                serviceRequestMessage->requestType = SERVER_RESPONSE_REQUEST;

                Logger::log("File path: %s\n", complete_file_name);
                FILE *search_file;

                Logger::log("File path: %s\n", complete_file_name);

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

            } else if (strcmp(serviceRequestMessage->requestString, "recursivelookup") == 0) {


                cout << "I am in recursive lookup" << endl;

                //value = read (connection, (char *) serviceRequestMessage, sizeof (serviceRequest));
                cout << "Server: client sent: " << serviceRequestMessage->requestString << endl;

                //int connect = -1;
                cout << "Before" << endl;
                char *file_content = searchFile(serviceRequestMessage->payload);
                cout << "After" << endl;
                Logger::log("File content: %s",file_content);

                if (sizeof(file_content) > 0) { // If found
                    cout << "Found " << serviceRequestMessage->payload << endl;
                    cout << "File conent: " << file_content << endl;
                    strcpy(serviceRequestMessage->requestString, "found");
                    strcpy(serviceRequestMessage->payload, file_content);

                    serviceRequestMessage->requestType = SERVER_RESPONSE_REQUEST;

                    write(connection, (char *) serviceRequestMessage, sizeof(serviceRequest));

                } else {

                    int connect = -1;
                    int found =0;

                    for (int i = 0; i < node_index; i++) {

                        if (!findVisited(serviceRequestMessage, neighbors_list[i].host_name,
                                         neighbors_list[i].host_port)) {

                            connect = client.connectToANode(neighbors_list[i].host_name,
                                                   neighbors_list[i].host_port);
                            strcpy(serviceRequestMessage->requestString, "recursivelookup");
                            //strcpy(serviceRequestMessage.payload, filename);
                            serviceRequestMessage->requestType = CLIENT_QUERY_REQUEST;


                            if (connect == 0) {
                                char tmp_ip_port[BUFFER_SIZE];
                                sprintf(tmp_ip_port, "%s;%s;", neighbors_list[i].host_name,
                                        neighbors_list[i].host_port);

                                strcat(serviceRequestMessage->visitedNodeList, tmp_ip_port);
                                write(connect, (char *)&serviceRequestMessage, sizeof(serviceRequest));
                                found =1;
                            } else if (connect == -1 ) {

                                found =0;
                            }
                        }
                    }
                   cout << "File not found" << endl;
                }
            }



        } else if (requestType == CLIENT_PAYLOAD_SHARE) {
            cout << "Server: (payload) : " << serviceRequestMessage->payload << endl;

            update_list(serviceRequestMessage->payload);

        } else if (requestType == CLIENT_EXIT_REQUEST) {
            close(connection);
        }


    } // while exits
}


void Server::update_list(char *payload)
{
    char **tokens;
    tokens = utility.str_split(payload, ';');
    Utility utility;

    if (tokens)
    {
        int i, j;
        char new_neighbors[1024];

        for (i = 1; *(tokens + i); i+=2)
        {
            neighbors tmp_neighbor;


            strcpy(tmp_neighbor.host_name, *(tokens + i));
            strcpy(tmp_neighbor.host_port, *(tokens + (i+1)));

            int found = 0;
            for (j = 0; j < node_index; j++) {

                if ((strcmp(neighbors_list[j].host_name, tmp_neighbor.host_name) == 0)
                        && (strcmp(neighbors_list[j].host_port, tmp_neighbor.host_port) == 0)) {
                    found = 1;
                    break;
                }
            }

            if (!found) {
                strcpy(neighbors_list[node_index].host_name, tmp_neighbor.host_name);
                strcpy(neighbors_list[node_index].host_port, tmp_neighbor.host_port);
                node_index++;
            }
        }

        cout << "New neighbors " << new_neighbors << endl;
        cout<< "Entire List " << endl;
        for (j = 0; j < node_index; j++) {
            cout << neighbors_list[j].host_name << " " << neighbors_list[j].host_port << endl;
        }
        cout << new_neighbors;

        utility.saveNeighborsList();
    }
}


char *Server::searchFile(char *fileName)
{

    char complete_file_name[BUFFER_SIZE];
    //char file_content[BUFFER_SIZE];
    char file_conent[BUFFER_SIZE];
    strcat(complete_file_name, FILE_SEARCH_DIRECTORY);
    strcat(complete_file_name, "/");
    strcat(complete_file_name, fileName);


    FILE *search_file;

    Logger::log("File path: %s\n", complete_file_name);

    if (search_file = fopen(complete_file_name, "r")) { // file exists
        char file_buffer[1024];

        while (fgets(file_buffer, sizeof file_buffer, search_file) != NULL) {
            strcat(file_conent, file_buffer);
            cout << "Content of file: " << file_conent;
        }

        fclose(search_file);
    }

    return file_conent;
}


int Server::findVisited(serviceRequest *request, char *host, char *port)
{
    int found = 0;
    char **tokens;
    tokens = utility.str_split(request->visitedNodeList, ';');

    if (tokens) {
        int i, j;

        for (i = 1; *(tokens + i); i += 2) {
            neighbors tmp_neighbors;

            found = 0;

            strcpy(tmp_neighbors.host_name, *(tokens + i));
            strcpy(tmp_neighbors.host_port, *(tokens + (i+1)));

            if ((strcmp(tmp_neighbors.host_name, host) == 0) &&
                    (strcmp(tmp_neighbors.host_port, port) == 0)) {
                found = 1;
                break;
            }
        }
    }

    return found;
}
