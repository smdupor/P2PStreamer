/*
 * BasicServer.cpp
 *
 *  Created on: May 19, 2021
 *      Author: smdupor
 */


#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
 #include <arpa/inet.h>

#include "BasicServer.h"
#include "constants.h"


BasicServer::BasicServer(int port) {
	PORTNUM = port;
	newsockfd = -1;
	sockfd = -1;
}

BasicServer::~BasicServer() {
	close(newsockfd);
	close(sockfd);
}

sockinfo BasicServer::start(){
	int sockfd, newsockfd, portno;
	socklen_t clilen;

	struct sockaddr_in serv_addr, cli_addr;

	sockinfo sock_ret;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd<0){
		std::cout << "ERROR opening socket";
		sock_ret.socket = -1;
		return sock_ret;
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

	sock_ret.socket = (int) accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	struct sockaddr_in* ipV4Addr = (struct sockaddr_in*)&cli_addr;
	struct in_addr ipAddr = ipV4Addr->sin_addr;

	sock_ret.cli_addr = new char[INET_ADDRSTRLEN];

	inet_ntop( AF_INET, &ipAddr, sock_ret.cli_addr, INET_ADDRSTRLEN );



	if (newsockfd < 0){
		std::cout << "Error on accept";
	}

	return sock_ret;
}

int BasicServer::listener(){
	int sockfd, newsockfd, portno;
	socklen_t clilen;

	struct sockaddr_in serv_addr, cli_addr;

	sockinfo sock_ret;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd<0){
		std::cout << "ERROR opening socket";
		sock_ret.socket = -1;
		return -1;
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));

	portno = PORTNUM;

	serv_addr.sin_family = AF_INET;

	serv_addr.sin_port = htons(portno);

	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
		std::cout << "Error on bind";
		return -1;
	}

	std::cout << "LISTENING FOR CONNECTIONS\n";
	listen(sockfd,5);
	sleep(1);
	std::cout << "Listen Started\n";
	clilen = sizeof(cli_addr);

	while(1){
		std::cout << "accepting";
	sock_ret.socket = (int) accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	struct sockaddr_in* ipV4Addr = (struct sockaddr_in*)&cli_addr;
	struct in_addr ipAddr = ipV4Addr->sin_addr;

	sock_ret.cli_addr = new char[INET_ADDRSTRLEN];

	inet_ntop( AF_INET, &ipAddr, sock_ret.cli_addr, INET_ADDRSTRLEN );

	registration(sock_ret);

	if (newsockfd < 0){
		std::cout << "Error on accept";
		return -1;
	}


	}

	return 1;
}

sockinfo BasicServer::accepter(int sockfd){
	int newsockfd;
	socklen_t clilen;

	struct sockaddr_in serv_addr, cli_addr;

	sockinfo sock_ret;

	clilen = sizeof(cli_addr);

	sock_ret.socket = (int) accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	struct sockaddr_in* ipV4Addr = (struct sockaddr_in*)&cli_addr;
	struct in_addr ipAddr = ipV4Addr->sin_addr;

	sock_ret.cli_addr = new char[INET_ADDRSTRLEN];

	inet_ntop( AF_INET, &ipAddr, sock_ret.cli_addr, INET_ADDRSTRLEN );

	if (newsockfd < 0){
		std::cout << "Error on accept";
	}

	return sock_ret;
}

bool BasicServer::registration(sockinfo socket) {
	char * buffer[2048];
	char * response_buffer[1024];
	std::string response;

	bool keep_open = true;
	int n;

	bzero(buffer,2048);

	while(keep_open) {
		//	sleep(0.05);

			n = read(socket.socket, buffer, 2047);
			if (n < 0)
				std::cerr << "ERROR reading from socket";
			if(std::strstr((const char *) buffer, kCliRegister) == (char *)buffer){
				std::cout << "registration string found";
				bzero(response_buffer, 1024);
/*				memcpy(response_buffer, cliRegAck, 4);
				response_buffer[5]= " ";
				response_buffer[6]*/
				response = std::string(kCliRegAck) + " " + std::string(socket.cli_addr) + "  Cookie 2";
				n = write(socket.socket, response.c_str(), std::strlen(response.c_str()));
				std::cout  << response;
				close(socket.socket);
				keep_open = false;
			}
			else
			{
				std::cout << "If statement failed";
				keep_open = false;
			}
	}
	return true;
}
