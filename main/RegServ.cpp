//============================================================================
// Name        : RS.cpp
// Author      : Stevan Dupor
// Version     : 0.1
// Description : Main file for Registration Server for P2P-DI
//============================================================================

#include "constants.h"
#include "RegistrationServer.h"

int main(void) {
	RegistrationServer server = RegistrationServer("NoLogFile.txt", true);

	server.start();

	return EXIT_SUCCESS;
}

