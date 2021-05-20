/*
 * FileReceiver.cpp
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

#include "FileReceiver.h"

FileReceiver::FileReceiver() {
	// TODO Auto-generated constructor stub

}

FileReceiver::~FileReceiver() {
	// TODO Auto-generated destructor stub
}

void FileReceiver::error(const char *msg){
	std::cout << msg;
	exit(0);
}

int FileReceiver::receive(char *fileName, int sockfd){
	char buffer[1024];
	int n=0, nlast=0;
	int counter = 0;
	int COUNT_TIMEOUT = 4000096;
	std::ofstream OutFile(fileName);

	bzero(buffer,1024);
	while(1){
		n = read(sockfd, buffer, 1023);
		if (n < 0)
			error("ERROR reading from socket");

		OutFile << buffer;
		bzero(buffer,1024);
		if( counter == COUNT_TIMEOUT){
			break;
			printf("TIMED OUT\n");
		}
		else if (nlast==n){
			counter++;
		}
		else{
			counter=0;
			printf("%s\n", buffer);
		}
	}
	OutFile.close();
	close(sockfd);
	return 0;
}
