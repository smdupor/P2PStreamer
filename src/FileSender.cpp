/*
 * FileSender.cpp
 *
 *  Created on: May 20, 2021
 *      Author: smdupor
 */

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

#include "FileSender.h"

FileSender::FileSender() {
	// TODO Auto-generated constructor stub

}

FileSender::~FileSender() {
	// TODO Auto-generated destructor stub
}


void FileSender::error(const char *msg){
	std::cout << msg;
	exit(0);
}

int FileSender::send(char *fileName, int sockfd){
	char buffer[1024];
	int n=0;
	std::ifstream InFile(fileName);

	bzero(buffer,1024);
	while(InFile.getline(buffer,1024)){
		buffer[strlen(buffer)] ='\n';
		n = write(sockfd, buffer, strlen(buffer));
		if (n < 0)
			error("ERROR reading from socket");
		printf("%s\n", buffer);
		bzero(buffer,1024);
	}
	InFile.close();
	close(sockfd);
	return 0;
}
