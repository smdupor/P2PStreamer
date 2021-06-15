/**
 * RegServ
 * main() top-level method for the Registration Server (aka tracker) in the P2P file sharing ecosystem.
 *
 * Created on: May 24th, 2021
 * Author: smdupor
 */

#include <thread>
#include "RegistrationServer.h"

int main(int argc, char *argv[]) {
   bool verbosity = false;
   // handle choice for verbose mode
   if (argc == 2 && *argv[2] == 'v'){
      verbosity = true;
   }

   // Tell user we're booting up
   NetworkCommunicator::info("Starting Registration Server.\n");

   // Initialize the server
	RegistrationServer server = RegistrationServer("NoLogFile.txt", verbosity);

	// Fork off the ttl keepalive continuous timing loop
   std::thread ttl_dec_thread = std::thread(&RegistrationServer::ttl_decrementer, &server);
   ttl_dec_thread.detach();

   // Boot the server continuous loop
   server.start();

   // Say goodbye and exit.
	NetworkCommunicator::info("********** All Hosts are gone; Exit clean/success ************\n");
	return EXIT_SUCCESS;
}

