#include "communication.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <cstdio>

using namespace std;


Communication::Communication() {
}

int  Communication::client_connection_accept(int *socketdesc) {
    int connection;
    connection = accept(*socketdesc, NULL, NULL);

    if(connection < 0){
        cout << " Server: Error in accept" << endl;
        exit(0);
    }
    return  connection;
}

/* Call this function multiple times until a connection is possible from our node or user input */
int Communication::create_client_connection(int *socketdesc, char *hostname, char *portnum) {

    int connection;             // Return value from connect()
    struct addrinfo *myinfo;    // Address info


    // Set up the address record
    if (getaddrinfo(hostname, portnum, NULL, &myinfo) != 0) {
        cout << "Client: Error getting address" << endl;
        exit(0);
    }

    connection = connect(*socketdesc, myinfo->ai_addr, myinfo->ai_addrlen);
    if (connection < 0) {
        cout << "Client: Error in connect" << endl;
        //exit(0);
    }
    return connection;
}


int Communication::create_server_connection(int *socketdesc, char *portnum, int upper_bound_port) {

    struct  addrinfo *myinfo;    // address record

    // Set up the address record for this machine
    int portint = atoi(portnum);

    // Use connection, not sockdesc, for read/write
    // get the portnum
    // Create the socket with domain, type, protocol as
    //    internet, stream, default
    *socketdesc = socket (AF_INET, SOCK_STREAM, 0);

    if (*socketdesc < 0)
    {
        cout << "Server: Error creating socket" << endl;
        exit (0);
    }

    while (portint < upper_bound_port)
    {

        if (getaddrinfo("0.0.0.0", portnum, NULL, &myinfo) != 0)
        {
            cout << "Server: Error getting address" << endl;
            exit (0);
        }			// if

        // Bind the socket to an address
        if (bind (*socketdesc, myinfo->ai_addr, myinfo->ai_addrlen) < 0)
        {
            cout << "Server: Error binding to socket. Let's get new port address" <<
                    endl;

            portint = atoi (portnum);
            portint++;
            //itoa(portint, portnum, 10);
            sprintf (portnum, "%d", portint);
            cout << "Server: Port changed to " << portnum << endl;

            continue;
        }
        else
        {
            break;
        }
    }				// while ends

    // if could not get a port exit
    if (portint >= upper_bound_port)
    {
        cout << "Server: Could not find a portnum to bind." << endl;
        exit(-1);
    }

    // Now listen to the socket
    if (listen (*socketdesc, 1) < 0)
    {
        cout << "Server: Error in listen" << endl;
        exit (0);
    }				// if

    return portint;
}
