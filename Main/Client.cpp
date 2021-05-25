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

#include "FileSender.h"
#include "FileReceiver.h"
#include "BasicClient.h"
#include "BasicServer.h"

int main(void) {
	int choice, sockfd;
		char *serverID = new char[1024];
		char *outFileName = (char *) "./Downloads/hardfilename";
		char *inFileName = (char *) "./Downloads/inhardfile";
		std::cout << "Server (1) Client (2): ";
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