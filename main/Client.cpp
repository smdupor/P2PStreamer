//============================================================================
// Name        : P2P.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <netdb.h>
#include <unistd.h>

#include "FileSender.h"
#include "FileReceiver.h"
#include "BasicClient.h"
#include "BasicServer.h"
#include "constants.h"

int main(void) {
	int choice, n, sockfd;
		char *serverID = new char[1024];
		char *outFileName = (char *) "./Downloads/hardfilename";
		//char *inFileName = (char *) "./Downloads/inhardfile";
		//char *testMessage = (char *) "REG NEW";
		std::string testMessage = std::string(kCliRegister) + " NEW";

		char *buffer = new char[2048];

		bzero(buffer, 2048);

		std::cout << "Server (1) Client (2): ";
		std::cin >> choice;
		if(choice == 1){
			//BasicServer basic(65432);
			//sockfd = basic.start();
			//FileReceiver fr;

			//fr.receive(outFileName, sockfd);
		}
		else if (choice == 2){
			BasicClient basicc(65432);
			std::cout << "Enter Server ID: ";
			std::cin >> serverID;
			std::cout << serverID;
			sockfd = basicc.start(serverID);
			n = write(sockfd, testMessage.c_str(), strlen(testMessage.c_str()));
			sleep(2);
			n = read(sockfd, buffer, 2048);
			std::cout << buffer;
			close(sockfd);
			/*FileSender fs;
			fs.send(inFileName, sockfd);*/
		}

		return EXIT_SUCCESS;
}
