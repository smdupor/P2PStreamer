/**
 * Client Main:
 * main() top-level method for the P2P Client system. Starts all functionality for the "Clients" in the Peer-Peer
 * filesharing ecosystem.
 *
 * Created on: May 24th, 2021
 * Author: smdupor
 */

#include <iostream>
#include <thread>

#include "P2PClient.h"

void initialization_interaction(int argc, char *const *argv, char &choose, bool &verbosity, std::string &server_id);

int main(int argc, char *argv[]) {
   if(argc < 3) {
      NetworkCommunicator::error("INVALID ARGUMENTS. Run command as: ./Client <letter code> <Registration Server Hostname>, "
                                 "e.g. './Client a 192.168.1.31\n");
      return EXIT_FAILURE;
   }
   char choose;
   bool verbosity;
   std::string server_id;

   // Communicate with the user about using "localhost" loopbacks if necessary
   initialization_interaction(argc, argv, choose, verbosity, server_id);

   // Set up logging based on the client code and initialize the client object
   std::string logfile = "logs/" + std::string(argv[1]) + ".csv";
   P2PClient client = P2PClient(server_id, logfile, verbosity);

   // Advise the user we are booting.
   std::string start_msg =  "Starting Client with Code:   " + std::to_string(choose) + " \n";
   NetworkCommunicator::info(start_msg);

   // Boot appropriate client based on user's choice
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

   // Fork off the keep_alive continuous loop thread
   std::thread keep_alive_thread = std::thread(&P2PClient::keep_alive, &client);
   keep_alive_thread.detach();

   // Fork off the downloader ("client") component thread
   std::thread downloader_thread = std::thread(&P2PClient::downloader, &client);
   downloader_thread.detach();

   // Set up  and start the listener ("file server") in the main thread
   socklen_t clilen;
   struct sockaddr_in cli_addr;
   int listen_socket = client.listener(client.get_port());

   // Continuously handle new incoming connections and fork off acceptance threads for each new connection. Unblocked by a
   // callback from the registration server upon leaving.
   while(client.get_system_on()){
      int new_sockfd = (int) accept(listen_socket, (struct sockaddr *) &cli_addr, &clilen);
      std::thread accept_thread(&P2PClient::accept_download_request, &client, new_sockfd);
      accept_thread.detach();
   }

   // Close the listener and wait for all remaining socket transactions to fully wind down in the API.
   close(listen_socket);
   std::this_thread::sleep_for(std::chrono::milliseconds(500));

   //Say goodbye and exit.
   NetworkCommunicator::info("***************System is exiting successfully***********\n");
		return EXIT_SUCCESS;
}

void initialization_interaction(int argc, char *const *argv, char &choose, bool &verbosity, std::string &server_id) {
   choose= argv[1][0];
   verbosity= false;
   server_id= std::string(argv[2]);
   if(server_id == "127.0.0.1" || server_id == "localhost") {
      NetworkCommunicator::warning("You have selected a localhost loopback address for the registration server.\n"
                                   "Your publicly-available IP address will not be detected properly, and communication\n"
                                   "with external clients will fail, unless ALL clients are running on the localhost.\n\n");
      NetworkCommunicator::error("Are you sure you want to continue with the localhost loopback adapter? (y/n): ");
      char ip_choice;
      std::cin >> ip_choice;
      if(ip_choice=='n'){
         NetworkCommunicator::warning("Enter the new hostname/IP address: ");
         std::cin >> server_id;
      }
   }

   if (argc == 4 && *argv[4] == 'v'){
      verbosity = true;
   }
}
