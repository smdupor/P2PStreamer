//============================================================================
// Name        : RS.cpp
// Author      : Stevan Dupor
// Version     : 0.1
// Description : Main file for Registration Server for P2P-DI
//============================================================================

#include <thread>
#include "constants.h"
#include "RegistrationServer.h"

int main(void) {
	RegistrationServer server = RegistrationServer("NoLogFile.txt", true);
   std::vector<std::unique_ptr<std::thread>> threads;

   std::thread ttl_decrementer = std::thread(&RegistrationServer::ttl_decrementer, &server);
	server.start();

	return EXIT_SUCCESS;
}

