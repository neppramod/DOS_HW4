#ifndef COMMUNICATION_H
#define COMMUNICATION_H

class Communication{
private:
    Communication();
    static Communication *_communication;
public:
    static Communication *commucationInstance();
    //Communication();
    /* Server: Accept a client's connection and return value for connection*/
    int client_connection_accept(int *socketdesc);
    /* Binding the server with port number and return used port number */
    int create_server_connection(int *socketdesc, char *portnum, int upper_bound_port );

    /* Create communication for client. Returns connection stat*/
    int create_client_connection(int *socketdesc, char *hostname, char *portnum );

    /* Client: Called once, thus out from create_client_connection */
    void create_socket_descriptor(int *sockdesc);

};

#endif
