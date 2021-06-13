//============================================================================
// Name        : RS.cpp
// Author      : Stevan Dupor
// Version     : 0.1
// Description : Main file for Registration Server for P2P-DI
//============================================================================

#include <thread>
#include "RegistrationServer.h"

int main(void) {
	RegistrationServer server = RegistrationServer("NoLogFile.txt", true);


   std::thread ttl_dec_thread = std::thread(&RegistrationServer::ttl_decrementer, &server);
   ttl_dec_thread.detach();

   server.start();
	//std::thread serv_thread(&RegistrationServer::start, &server);
	//serv_thread.detach();

	//ttl_dec_thread.join();
	std::cout << "All Hosts are gone; Exit clean/success.";

	return EXIT_SUCCESS;
}

