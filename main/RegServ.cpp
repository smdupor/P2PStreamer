//============================================================================
// Name        : RS.cpp
// Author      : Stevan Dupor
// Version     : 0.1
// Description : Main file for Registration Server for P2P-DI
//============================================================================

#include <thread>
#include "RegistrationServer.h"

int main(int argc, char *argv[]) {
   bool verbosity = false;
   if (argc == 2 && *argv[2] == 'v'){
      verbosity = true;
   }
   NetworkCommunicator::info("Starting Registration Server.\n");

	RegistrationServer server = RegistrationServer("NoLogFile.txt", verbosity);

   std::thread ttl_dec_thread = std::thread(&RegistrationServer::ttl_decrementer, &server);
   ttl_dec_thread.detach();

   server.start();

	NetworkCommunicator::info("********** All Hosts are gone; Exit clean/success ************\n");
	return EXIT_SUCCESS;
}

