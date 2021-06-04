//============================================================================
// Name        : P2P.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <iostream>

#include "P2PClient.h"

int main(void) {
	std::string server_id;

   std::cout << "Enter Server ID: ";
   std::cin >> server_id;

   P2PClient client = P2PClient(server_id, "logs/logfileexample.txt", true);

   client.start();

		return EXIT_SUCCESS;
}
