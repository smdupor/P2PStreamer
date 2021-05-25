//============================================================================
// Name        : RS.cpp
// Author      : Stevan Dupor
// Version     : 0.1
// Description : Main file for Registration Server for P2P-DI
//============================================================================

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "FileSender.h"
#include "FileReceiver.h"
#include "BasicClient.h"
#include "BasicServer.h"

int main(void) {
	int choice, sockfd;
	char *serverID = new char[1024];
	char *outFileName = (char *) "./Downloads/hardfilename";
	char *inFileName = (char *) "./Downloads/inhardfile";
	std::cout << "Server (1) (IMPLIED) Client (2): ";
	std::cin >> choice;
	if(choice == 1){
		BasicServer basic(65432);
		sockfd = basic.start();
		FileReceiver fr;

		fr.receive(outFileName, sockfd);
	}
	else if (choice == 2){
		BasicClient basicc(65432);
		std::cout << "Enter Server ID: ";
		std::cin >> serverID;
		std::cout << serverID;
		sockfd = basicc.start(serverID);
		FileSender fs;
		fs.send(inFileName, sockfd);
	}

	return EXIT_SUCCESS;
}

