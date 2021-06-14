/*
 * RegistrationServer.cpp
 *
 *  Created on: May 28, 2021
 *      Author: smdupor
 */

#include "RegistrationServer.h"

std::mutex peerlist_lock;

// Constructor to initialize the registration server and set up logging
RegistrationServer::RegistrationServer(std::string logfile, bool verbose_debug) : NetworkCommunicator() {
   // Initialize instance variables
   log = logfile;
   latest_cookie = 1;
   lock = false;
   port = kControlPort;
   debug = verbose_debug;
   start_time = (const time_t) std::time(nullptr);
   reverse = false;
   system_on = true;
}

RegistrationServer::~RegistrationServer() {
   //Destructor
}

// Starts the registration server, creates a socket and listens/accepts/handles new connections
void RegistrationServer::start() {
   int sockfd; // socket descriptor
   socklen_t clilen; //client length
   struct sockaddr_in cli_addr; //socket addresses
   sockinfo accepted_socket; // Values passed on once a connection is accepted

   sockfd = listener(this->port);

   clilen = sizeof(cli_addr);

   // Loop continuously to accept new connections
   while (system_on) {

      //accept the connection
      accepted_socket.socket = (int) accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

      //convert the hostname to a useable datatype
      struct sockaddr_in *ipV4Addr = (struct sockaddr_in *) &cli_addr;
      struct in_addr ipAddr = ipV4Addr->sin_addr;
      accepted_socket.cli_addr = new char[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &ipAddr, accepted_socket.cli_addr, INET_ADDRSTRLEN);

      if (system_on) {      // Handle the incoming request
         accept_reg(accepted_socket);
      }

      if (accepted_socket.socket < 0) {
         verbose("Error on accepting connection");
      }
   }

   close(sockfd);
   std::this_thread::sleep_for(std::chrono::seconds(2));
}

int RegistrationServer::accept_reg(sockinfo sock) {
   std::string in_message, out_message;
   in_message = receive(sock.socket);

   std::vector<std::string> messages = split((const std::string &) in_message, '\n');

   for (std::string &message : messages) {
      // Split the string by the delimiter so we can more easily use the message data
      std::vector<std::string> tokens = split((const std::string &) message, ' ');

      //Handle client registration
      if (tokens[0] == kCliRegister) {   // First message when client wishes to register
         if (tokens[1] == "NEW") {
            // This is a new registration, handle it by adding the client and replying with ack
            out_message = kCliRegAck + create_new_peer(sock);
         } else {
            // This is a returning registration, and tokens[2] will contain the cookie.
            PeerNode &pn = *std::find_if(peers.begin(), peers.end(), [&](PeerNode node) {
               return node.equals(stoi(tokens[2]));
            });
            pn.set_inactive();
            pn.increment_reg_count();
            out_message = kCliRegAck + pn.to_msg();
         }
         transmit(sock.socket, out_message);

         // Now, reply with all other active members of the list
         if (reverse) {
            for (std::list<PeerNode>::reverse_iterator rit = peers.rbegin(); rit != peers.rend(); ++rit) {
               if (rit->active()) {
                  out_message = kPeerListItem + rit->to_msg();
                  transmit(sock.socket, out_message);
               }
            }
            reverse = false;
         }
         for (PeerNode &p : peers) {
            if (p.active()) {
               out_message = kPeerListItem + p.to_msg();
               transmit(sock.socket, out_message);
            }
            reverse = true;
         }

         // Finally, say "DONE" and close.
         out_message = kDone + " \n\n";
         transmit(sock.socket, out_message);
      } else if (tokens[0] == kKeepAlive) { // This is a keepalive message
         // tokens [2] will contain the cookie

         PeerNode &p = *std::find_if(peers.begin(), peers.end(), [&](PeerNode node) {
            return node.equals(stoi(tokens[COOKIE]));
         });

         p.keepAlive();
         out_message = kDone + " \n\n";
         transmit(sock.socket, out_message);
      } else if (tokens[0] == kLeave) { // Client is leaving the system
         // tokens[2] will contain the cookie
         PeerNode &p = *std::find_if(peers.begin(), peers.end(), [&](PeerNode node) {
            return node.equals(stoi(tokens[COOKIE]));
         });
         p.leave();
         verbose(p.to_string() + " Is Leaving\n");
         int killsockfd = outgoing_connection(p.get_address(), p.get_port());
         out_message = kDone + " \n\n";
         transmit(killsockfd, out_message);
         std::this_thread::sleep_for(std::chrono::microseconds(100));
         close(killsockfd);

         transmit(sock.socket, out_message);
         std::this_thread::sleep_for(std::chrono::milliseconds(100));
         // Shutdown the reg serv if experiment is over & everyone is gone.
         if (!std::any_of(peers.begin(), peers.end(), [&](PeerNode &p) { return p.active(); })) {
            system_on = false;
            return 1;
         }
      } else if (tokens[0] == kGetPeerList) { // Registered client wants the list of peers
         std::find_if(peers.begin(), peers.end(), [&](PeerNode node) {
            return node.equals(stoi(tokens[COOKIE]));
         })->keepAlive();

         for (PeerNode p : peers) { //Transmit each
            out_message = kPeerListItem + p.to_msg();
            transmit(sock.socket, out_message);
         }
         //Transmit done and exit loop.
         out_message = kDone + " \n\n";
         transmit(sock.socket, out_message);
      } else {
         error("We received an invalid message. Dropping connection.");
      }
   } //foreach
   return 0;
}

std::string RegistrationServer::create_new_peer(sockinfo sock) {
   //create a new peernode for the new registrant
   PeerNode p = PeerNode(std::string(sock.cli_addr), latest_cookie, kControlPort + latest_cookie);
   ++latest_cookie;
   p.set_inactive();

   peerlist_lock.lock();
   // Add to list of peers
   peers.push_back(p);
   peerlist_lock.unlock();

   // Return the messagized version
   return p.to_msg();
}

void RegistrationServer::ttl_decrementer() {
   int seconds_counter = 0;
   while (system_on) {
      if (seconds_counter < kTTLDec) {
         ++seconds_counter;
         std::this_thread::sleep_for(std::chrono::seconds(1));
      } else {
         for (PeerNode &p : peers) {
            p.decTTL(kTTLDec);
         }
         seconds_counter = 0;
      }
      for (PeerNode &p : peers) {
         if (!p.active()) {
            p.increment_drop_counter();
         }
      }
      peerlist_lock.lock();
      peers.remove_if([&](PeerNode &p) { return p.drop_entry(); });
      peerlist_lock.unlock();

   }

}