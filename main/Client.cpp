//============================================================================
// Name        : P2P.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <iostream>
#include <thread>

#include "P2PClient.h"

int main(int argc, char *argv[]) {
   int listen_socket, newsockfd;
   socklen_t clilen; //client length
   struct sockaddr_in serv_addr, cli_addr; //socket addresses

   std::vector<std::unique_ptr<std::thread>> threads;

   if(argc != 3)
      return EXIT_FAILURE;

   char choose = argv[1][0];
   std::string server_id = std::string(argv[2]);
/*
   std::cout << "Enter Server ID ('0' for localhost): ";
   std::cin >> server_id;*/
   if(server_id.length()==1)
      server_id = "localhost";

   P2PClient client = P2PClient(server_id, "logs/logfileexample.txt", true);

   //std::cout << "Which client would you like to simulate? (A, B, C, D, E):";
   //std::cin >> choose;

   std::cout << "Starting Client with Code:   " << choose<<"   \n";

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
      case 'f': client.start("conf/f.conf"); break;
      case 'F': client.start("conf/f.conf"); break;
      default: std::cout<<"Defaulting to Client A";client.start("conf/a.conf"); break;
   }

   std::thread keep_alive_thread = std::thread(&P2PClient::keep_alive, &client);
   keep_alive_thread.detach();

   std::thread downloader_thread = std::thread(&P2PClient::downloader, &client);
   downloader_thread.detach();

   listen_socket = client.listener(client.get_port());

   while(client.get_system_on()){
      newsockfd = (int) accept(listen_socket, (struct sockaddr *) &cli_addr, &clilen);
      std::thread accept_thread(&P2PClient::accept_download_request, &client, newsockfd);
      accept_thread.detach();
   }

  // keep_alive_thread.join();

   std::cout << "*********************System is exiting successfully*********************\n";
		return EXIT_SUCCESS;
}
