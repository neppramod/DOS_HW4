#ifndef MESSAGE_H
#define MESSAGE_H

//
// message.h
//
// Defines a simple struct to hole a message

struct serviceRequest
{
    char domainName[256];	// requester's domain name
    int portNumber;	// requester's port number
    int requestType;	// 0=new client, 1=client is existing, 2=query, 3=response
    char requestString[32]; // Parameters of request
    int requestID;		// Unique ID
    char payload[256];	// Other request information, to be determined
    char message[512];  // message transferred
    char visitedNodeList[10000];

};

#endif // MESSAGE_H
