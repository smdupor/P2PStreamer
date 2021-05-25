/*
 * BasicServer.cpp
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

#include "BasicServer.h"

BasicServer::BasicServer(int port) {
	PORTNUM = port;
	newsockfd = -1;
	sockfd = -1;
}

BasicServer::~BasicServer() {
	close(newsockfd);
	close(sockfd);
}

int BasicServer::start(){
	int sockfd, newsockfd, portno;
	socklen_t clilen;

	struct sockaddr_in serv_addr, cli_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd<0){
		std::cout << "ERROR opening socket";
		return -1;
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));

	portno = PORTNUM;

	serv_addr.sin_family = AF_INET;

	serv_addr.sin_port = htons(portno);

	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
		std::cout << "Error on bind";
	}

	std::cout << "LISTENING FOR CONNECTIONS\n";
	listen(sockfd,5);

	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if (newsockfd < 0){
		std::cout << "Error on accept";
	}

	return newsockfd;
}
