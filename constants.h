#ifndef  CONSTANTS_H
#define  CONSTANTS_H


// 0=new client, 1=exit, 2=query, 3=response, 4=share
enum {
    CLIENT_NEW_CONNECTION_REQUEST,
    CLIENT_EXIT_REQUEST,
    CLIENT_QUERY_REQUEST,
    SERVER_RESPONSE_REQUEST,
    CLIENT_PAYLOAD_SHARE
};

#define PORT_FILE_NAME "portnums"
#define NEIGHBORS_FILE_LIST_NAME "neighbors" // Neighbors list file
#define BUFFER_SIZE 100

#endif