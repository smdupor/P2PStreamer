/*
 * BasicClient.cpp
 *
 *  Created on: May 19, 2021
 *      Author: smdupor
 */

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

#include "BasicClient.h"

BasicClient::BasicClient(int portNum) {
	PORTNUM = portNum;
	sockfd = -1;

}

BasicClient::~BasicClient() {
	close(sockfd);
}

void BasicClient::error(const char *msg){
	std::cout << msg;
	exit(0);
}

int BasicClient::start(const char *id){
	int portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[1024];

	portno = PORTNUM;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	server = gethostbyname(id);

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

	serv_addr.sin_port = htons(portno);
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR connecting");

	return sockfd;
}


