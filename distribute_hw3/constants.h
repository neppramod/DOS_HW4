#ifndef CONSTANTS_H
#define CONSTANTS_H

// 0=new client, 1=exit, 2=query, 3=response, 4=share
enum {
    CLIENT_NEW_CONNECTION_REQUEST,
    CLIENT_EXIT_REQUEST,
    CLIENT_QUERY_REQUEST,
    SERVER_RESPONSE_REQUEST,
    CLIENT_PAYLOAD_SHARE
};

// Default port number if user does not specify it
#define PORT_FILE_NAME "portnums"
#define MAXNODES 80
#define NEIGHBORS_LIST_FILE_NAME "neighbors" // Neighbors list file
#define CLIENT_DOMAIN_NAME_LENGTH 256  // client's domain name size from serviceRequest
#define SERVER_NODE_LIST_SIZE 23040 // 256x90
#define BUFFER_SIZE 81
#define LARGE_BUFFER_SIZE 1024
#define FILE_SEARCH_DIRECTORY  "content"

#endif // CONSTANTS_H
