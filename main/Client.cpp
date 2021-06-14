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
   if(argc < 3) {
      NetworkCommunicator::error("INVALID ARGUMENTS. Run command as: ./Client <letter code> <Registration Server Hostname>, "
                                 "e.g. './Client a 192.168.1.31\n");
      return EXIT_FAILURE;
   }

   char choose = argv[1][0];
   bool verbosity = false;
   std::string server_id = std::string(argv[2]);

   if(server_id == "127.0.0.1" || server_id == "localhost") {
      NetworkCommunicator::warning("You have selected a localhost loopback address for the registration server. \n"
                                   "Your publicly-available IP address will not be detected properly unless ALL clients\n"
                                   "are running on the localhost, and this client will not be able to communicate with"
                                   "clients on other hosts. Do you want to select a different IP/Hostname? (y/n): ");
      char ip_choice;
      std::cin >> ip_choice;
      if(ip_choice=='y'){
         NetworkCommunicator::warning("Enter the new hostname/IP address: ");
         std::cin >> server_id;
      }
   }

   if (argc == 4 && *argv[4] == 'v'){
      verbosity = true;
   }

   std::string logfile = "logs/" + std::string(argv[1]) + ".csv";
   P2PClient client = P2PClient(server_id, logfile, verbosity);

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
      default:
         NetworkCommunicator::error("INVALID ARGUMENTS. Run command as: ./Client <letter code> <Registration Server Hostname>, "
                                    "e.g. './Client a 192.168.1.31\n");
         return EXIT_FAILURE;
         break;
   }

   std::thread keep_alive_thread = std::thread(&P2PClient::keep_alive, &client);
   keep_alive_thread.detach();

   std::thread downloader_thread = std::thread(&P2PClient::downloader, &client);
   downloader_thread.detach();

   socklen_t clilen;
   struct sockaddr_in cli_addr;
   int listen_socket = client.listener(client.get_port());

   while(client.get_system_on()){
      int new_sockfd = (int) accept(listen_socket, (struct sockaddr *) &cli_addr, &clilen);
      std::thread accept_thread(&P2PClient::accept_download_request, &client, new_sockfd);
      accept_thread.detach();
   }

   close(listen_socket);

   NetworkCommunicator::warning("***************System is exiting successfully***********\n");
		return EXIT_SUCCESS;
}
