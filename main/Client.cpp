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
   char choose;

   std::cout << "Enter Server ID ('0' for localhost): ";
   std::cin >> server_id;
   if(server_id.length()==1)
      server_id = "localhost";

   P2PClient client = P2PClient(server_id, "logs/logfileexample.txt", true);

   std::cout << "Which client would you like to simulate? (A, B, C, D, E):";
   std::cin >> choose;

   switch(choose) {
      case 'a': client.start("conf/a.conf"); break;
      case 'A': client.start("conf/a.conf"); break;
      case 'b': client.start("conf/b.conf"); break;
      case 'B': client.start("conf/b.conf"); break;
      case 'c': client.start("conf/c.conf"); break;
      case 'C': client.start("conf/c.conf"); break;
      case 'd': client.start("conf/d.conf"); break;
      case 'D': client.start("conf/d.conf"); break;
      case 'e': client.start("conf/e.conf"); break;
      case 'E': client.start("conf/e.conf"); break;
      default: std::cout<<"Defaulting to Client A";client.start("conf/a.conf"); break;
   }

   client.debug_print_hosts_and_files();

		return EXIT_SUCCESS;
}
