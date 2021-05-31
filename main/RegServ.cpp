//============================================================================
// Name        : RS.cpp
// Author      : Stevan Dupor
// Version     : 0.1
// Description : Main file for Registration Server for P2P-DI
//============================================================================

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <ctime>
#include <algorithm>
#include <unistd.h>

#include "constants.h"
//#include "BasicClient.h"
//#include "BasicServer.h"
#include "PeerNode.h"
#include "RegistrationServer.h"

int main(void) {
	RegistrationServer server = RegistrationServer("NoLogFile.txt", true);

	/*std::list <PeerNode> peers;
	BasicServer listener = BasicServer(controlPort);
	int n;
	sockinfo socket;


	char * buffer[2048];

	bzero(buffer,2048);
	while(1) {
		socket = listener.start();
		std::cout << "We found the remote IP to be: " << (std::string) socket.cli_addr;
		while(std::strlen((const char *) buffer) == 0) {
			sleep(0.05);

			n = read(socket.socket, buffer, 1023);
			if (n < 0)
				std::cerr << "ERROR reading from socket";
		}
		std::cout <<"We received the message: ";
		puts((const char *) buffer);
		n = listener.listener();
	}


	peers.push_back(PeerNode("asdfasdf", 1, 2));
	sleep(1);
	peers.push_back(PeerNode("jk;ljkli", 3, 4));
	sleep(1);
	std::for_each(peers.begin(), peers.end(), [](PeerNode node) {
		std::cout << node.toS() << '\n';
	});

	std::for_each(peers.begin(), peers.end(), [](PeerNode node) {
		if(node.equals("asdfasdf")) {
			std::cout << "Match Found: " << node.toS() << '\n';
	}});*/

	return EXIT_SUCCESS;
}

