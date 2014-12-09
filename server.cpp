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
#include <list>

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


        if(value != sizeof (serviceRequest))
        {
            cout << "Server: Error, read( ) returned a bad value" << endl;
        }

        requestType = serviceRequestMessage->requestType;


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

                //Logger::log("File path: %s\n", complete_file_name);
                FILE *search_file;

                //Logger::log("File path: %s\n", complete_file_name);

                if (search_file = fopen(complete_file_name, "r")) { // file exists
                    char file_buffer[1024];
                    char file_conent[1024];
                    while (fgets(file_buffer, sizeof file_buffer, search_file) != NULL) {
                        strcat(file_conent, file_buffer);
                    }

                    strcpy(serviceRequestMessage->requestString, "found");
                    strcpy(serviceRequestMessage->payload, file_conent);

                    fclose(search_file);
                }  else { // file does not exist
                    strcpy(serviceRequestMessage->requestString, "not found");
                    cout << "File does not exist" << endl;
                }

                write(connection, (char *) serviceRequestMessage, sizeof(serviceRequest));



            } else if (strcmp(serviceRequestMessage->requestString, "recursivelookup") == 0) { // For number 5


                char *file_content = searchFile(serviceRequestMessage->payload);

                if (strlen(file_content) > 0) { // If found

                    strcpy(serviceRequestMessage->payload, file_content);
                    write(connection, (char *) serviceRequestMessage, sizeof(serviceRequest));

                } else {

                    for (int i = 0; i < node_index; i++) {

                        if (findVisited (serviceRequestMessage->visitedNodeList, neighbors_list[i].host_name, neighbors_list[i].host_port) == 1) {
                                                    //cout << "Matched. Find another to connect" << endl;
                                                  } else {
                            // Currently doing for only one client
                            serviceRequest forwardRequestMessage;


                            strcpy(forwardRequestMessage.payload, serviceRequestMessage->payload);
                            forwardRequestMessage.requestType = CLIENT_QUERY_REQUEST;

                            strcpy(forwardRequestMessage.requestString, "recursivelookup");

                            int conn_for = -1;
                            Communication *communication = Communication::commucationInstance ();
                            int for_sockdesc;
                            communication->create_socket_descriptor (&for_sockdesc);

                            conn_for = communication->create_client_connection (&for_sockdesc, neighbors_list[i].host_name, neighbors_list[i].host_port);

                            if (conn_for == 0) {

                                write(for_sockdesc, (char *)&forwardRequestMessage, sizeof(serviceRequest));

                                int rec_return = read(for_sockdesc, (char *) &forwardRequestMessage, sizeof(serviceRequest));


                                if (rec_return > 0) {
                                    strcpy(serviceRequestMessage->payload, forwardRequestMessage.payload);
                                    write(connection, (char *) serviceRequestMessage, sizeof(serviceRequest));
                                }
                              }
                          }
                      } // for loop ends

                }
            }  else if (strcmp(serviceRequestMessage->requestString, "returnneighbors") == 0) { // For number 6



                char tmp_neighbor_arr[BUFFER_SIZE];

                for (int i = 0; i < node_index; i++) {

                    if (findVisited (serviceRequestMessage->visitedNodeList, neighbors_list[i].host_name, neighbors_list[i].host_port) == 1) {
                                                //cout << "Matched. Find another to connect to" << endl;
                                              } else {
                        serviceRequest forwardRequestMessage;

                        forwardRequestMessage.requestType = CLIENT_QUERY_REQUEST;
                        strcpy(forwardRequestMessage.requestString, "returnneighbors");

                        int conn_for = -1;
                        Communication *communication = Communication::commucationInstance ();
                        int for_sockdesc;
                        communication->create_socket_descriptor (&for_sockdesc);

                        conn_for = communication->create_client_connection (&for_sockdesc, neighbors_list[i].host_name, neighbors_list[i].host_port);

                        if (conn_for == 0) {



                            strcat(tmp_neighbor_arr, neighbors_list[i].host_name);
                            strcat(tmp_neighbor_arr, ";");
                            strcat(tmp_neighbor_arr, neighbors_list[i].host_port);

                            strcat(serviceRequestMessage->visitedNodeList, ";");
                            strcat(serviceRequestMessage->visitedNodeList, tmp_neighbor_arr);
                        }

                      }

                }


                strcpy(serviceRequestMessage->payload, tmp_neighbor_arr);
                write(connection, (char *) serviceRequestMessage, sizeof(serviceRequest));

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

        //cout << "New neighbors " << new_neighbors << endl;
        cout<< "Entire List " << endl;
        for (j = 0; j < node_index; j++) {
            cout << neighbors_list[j].host_name << " " << neighbors_list[j].host_port << endl;
        }
        //cout << new_neighbors;

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

    //Logger::log("File path: %s\n", complete_file_name);

    if (search_file = fopen(complete_file_name, "r")) { // file exists

        //cout << "File found" << endl;
        char file_buffer[1024];

        while (fgets(file_buffer, sizeof file_buffer, search_file) != NULL) {
            strcat(file_conent, file_buffer);
            //cout << "Content of file: " << file_conent;
        }

        //cout << "Content of file: " << file_conent;

        fclose(search_file);
    } else { // file does not exist
        //Logger::log("I am inside file does not exist");
        cout << "File does not exist" << endl;
        strcpy(file_conent, "");
      }

    return file_conent;
}


int Server::findVisited(char *buffer, char *host, char *port)
{
    int found = 0;

    string visitedList(buffer);
    string hostStr(host);
    string portStr(port);

    //cout << "Trying to match '" << visitedList << "' with '" << hostStr << ";" << portStr << "'" << endl;

    //char* buffer = "this*is*a*simple*test";
    string delimiter = ";";
    string s(buffer);
    size_t pos = 0;
    string token;

    int i = 0; // toggle to set host or port

    string hostFound = "";
    string portFound = "";

    while ((pos = s.find(delimiter)) != std::string::npos) {
            token = s.substr(0, pos);
            cout<<token<<endl;
            s.erase(0, pos + delimiter.length());

            if (i == 0) {
                //hostFound = token;
                if (hostStr  == token)
                  hostFound = token;

                ++i;
              } else if (i == 1) {

                if (portStr == token)
                  portFound = token;

                --i;
              }
    }


    if (portStr == s)
      portFound = s;

    if (hostFound != "" && portFound != "") {
      cout << "Matched " << hostFound << ";" << portFound << endl;
      found = 1;
    }

    return found;
}
