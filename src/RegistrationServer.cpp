/**
 * RegistrationServer
 *
 * Contains all the functionality of the registration server (tracker) for the P2P Filesharing system. Holds the list of
 * peers and manages their availability and state.
 *
 *  Created on: May 28, 2021
 *      Author: smdupor
 */

#include "RegistrationServer.h"

std::mutex peerlist_lock; // Prevent data race between threads when accessing the peer list

/**
 * Constructor to initialize the server.
 *
 * @param logfile to write to if logging on the registration server is enabled
 * @param verbose_debug enables verbose output to the terminal
 */
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

/**
 * System destructor.
 */
RegistrationServer::~RegistrationServer() {
   //Destructor
}

/**
 * Boot up the registration server, start a listening socket, and begin looping as long as the system is on to handle
 * incoming requests.
 */
void RegistrationServer::start() {
   int sockfd;
   socklen_t clilen;
   struct sockaddr_in cli_addr;
   sockinfo accepted_socket; // Values passed on once a connection is accepted

   sockfd = listener(this->port);
   clilen = sizeof(cli_addr);

   // Loop continuously to accept new connections
   while (system_on) {

      //accept the connection
      accepted_socket.socket = (int) accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

      //convert the hostname to a useable IP address
      struct sockaddr_in *ipV4Addr = (struct sockaddr_in *) &cli_addr;
      struct in_addr ipAddr = ipV4Addr->sin_addr;
      accepted_socket.cli_addr = new char[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &ipAddr, accepted_socket.cli_addr, INET_ADDRSTRLEN);

      // Handle the request
      if (system_on) {
         accept_incoming_request(accepted_socket);
      }
      if (accepted_socket.socket < 0) {
         verbose("Error on accepting connection");
      }
   }
   close(sockfd);
   std::this_thread::sleep_for(std::chrono::seconds(2));
}

/**
 * Accepts all incoming requests from client hosts
 * @param socket_data Contains the socket file descriptor and the detected IP address of this client.
 * @return 0 on request complete with system still on, 1 on complete with system shutting down.
 */
int RegistrationServer::accept_incoming_request(sockinfo socket_data) {
   std::string in_message, out_message;
   in_message = receive(socket_data.socket);

   std::vector<std::string> messages = split((const std::string &) in_message, '\n');

   for (std::string &message : messages) {
      // Split the string by the delimiter so we can more easily use the message data
      std::vector<std::string> tokens = split((const std::string &) message, ' ');

      //Handle client registration
      if (tokens[CONTROL] == kCliRegister) {   // First message when client wishes to register
         handle_registration_request(socket_data, tokens);
      } else if (tokens[CONTROL] == kKeepAlive) { // This is a keepalive message
         handle_keepalive_request(socket_data, tokens);
      } else if (tokens[CONTROL] == kLeave) { // Client is leaving the system
         handle_leave_request(socket_data, tokens);
         // Shutdown the reg serv if experiment is over & everyone is gone.
         if (!std::any_of(peers.begin(), peers.end(), [&](PeerNode &p) { return p.active(); })) {
            system_on = false;
            return 1;
         }
      } else if (tokens[CONTROL] == kGetPeerList) { // Registered client wants the list of peers
         handle_get_peer_list_request(socket_data, tokens);
      } else {
         error("We received an invalid message. Dropping connection.");
      }
   } //foreach
   return 0;
}

/**
 * Handle a request for all the peers in the system.
 * @param socket_info Contains socket file descriptor and IP of the requestor
 * @param tokens tokenized message from the requestor (contains their cookie)
 */
void RegistrationServer::handle_get_peer_list_request(const sockinfo &socket_info, const std::vector<std::string> &tokens) {
   std::string out_message;
   // Update the keepalive timer of the requester
   std::find_if(peers.begin(), peers.end(), [&](PeerNode node) {
      return node.equals(stoi(tokens[COOKIE]));
   })->keepAlive();

   //Transmit each peer item to the requester
   for (PeerNode p : peers) {
      out_message = kPeerListItem + p.to_msg();
      transmit(socket_info.socket, out_message);
   }

   //Transmit done and exit loop.
   out_message = kDone + " \n\n";
   transmit(socket_info.socket, out_message);
}

/**
 * Handle a request to keep-alive (this) peer node
 * @param socket_info Contains socket file descriptor and IP of the requestor
 * @param tokens tokenized message from the requestor (contains their cookie)
 */
void RegistrationServer::handle_keepalive_request(const sockinfo &socket_info, const std::vector<std::string> &tokens) {
   std::string out_message;
   // Find client entry
   PeerNode &p = *std::find_if(peers.begin(), peers.end(), [&](PeerNode node) {
      return node.equals(stoi(tokens[COOKIE]));
   });

   //update keepalive timer, reply with "done"
   p.keepAlive();
   out_message = kDone + " \n\n";
   transmit(socket_info.socket, out_message);
}

/**
 * Handle a request for this peer to leave the system
 * @param socket_info Contains socket file descriptor and IP of the requestor
 * @param tokens tokenized message from the requestor (contains their cookie)
 */
void RegistrationServer::handle_leave_request(const sockinfo &socket_info, const std::vector<std::string> &tokens) {
   std::string out_message;
   //Find the client in the list
   PeerNode &p = *std::find_if(peers.begin(), peers.end(), [&](PeerNode node) {
      return node.equals(stoi(tokens[COOKIE]));
   });

   //Mark as leaving
   p.leave();
   verbose(p.to_string() + " Is Leaving\n");

   // Run a callback to the leaving client's listener to unblock their accept call.
   int killsockfd = outgoing_connection(p.get_address(), p.get_port());
   out_message = kDone + " \n\n";
   transmit(killsockfd, out_message);
   std::this_thread::sleep_for(std::chrono::microseconds(100));
   close(killsockfd);

   // Transmit "done" to complete leave transaction.
   transmit(socket_info.socket, out_message);
   std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

/**
 * Handle a request for this peer to join the system, and advise the peer of their publicly-acessible IP that we detected.
 *
 * @param socket_info Contains socket file descriptor and IP of the requestor
 * @param tokens tokenized message from the requestor (contains their cookie)
 */
void RegistrationServer::handle_registration_request(sockinfo &socket_info, const std::vector<std::string> &tokens) {
   std::string out_message;
   if (tokens[1] == "NEW") {
      // This is a new registration, handle it by adding the client and replying with ack
      out_message = kCliRegAck + create_new_peer(socket_info);
   } else {
      // This is a returning registration
      PeerNode &pn = *std::find_if(peers.begin(), peers.end(), [&](PeerNode node) {
         return node.equals(stoi(tokens[COOKIE]));
      });
      pn.set_inactive();
      pn.increment_reg_count();
      out_message = kCliRegAck + pn.to_msg();
   }
   transmit(socket_info.socket, out_message);

   // Now, reply with all other active members of the list. Alternate between a forward read and a reverse read
   // for each request to spread the load across oldest/newest peers.
   if (reverse) { // Do a reversed list
      for (std::list<PeerNode>::reverse_iterator rit = peers.rbegin(); rit != peers.rend(); ++rit) {
         if (rit->active()) {
            out_message = kPeerListItem + rit->to_msg();
            transmit(socket_info.socket, out_message);
         }
      }
      reverse = false;
   } else{ // Do a forward list
      for (PeerNode &p : peers) {
         if (p.active()) {
            out_message = kPeerListItem + p.to_msg();
            transmit(socket_info.socket, out_message);
         }
         reverse = true;
      }
   }

   // Finally, say "DONE" and close.
   out_message = kDone + " \n\n";
   transmit(socket_info.socket, out_message);
}

/**
 * Add a new peer to our peers database, update the cookies we can assign, and return a messagized confirmation to be
 * xmitted back to the client
 *
 * @param socket_info contains socket file descriptor and the IP of the requestor
 * @return A client-ack message to be transmitted to the client advising them of their external IP, and that they have
 *          been accepted to the system.
 */
std::string RegistrationServer::create_new_peer(sockinfo &socket_info) {
   //create a new peernode for the new registrant
   PeerNode p = PeerNode(std::string(socket_info.cli_addr), latest_cookie, kControlPort + latest_cookie);
   ++latest_cookie;
   p.set_inactive();

   peerlist_lock.lock();
   // Add to list of peers
   peers.push_back(p);
   peerlist_lock.unlock();

   // Return the messagized version
   return p.to_msg();
}

/**
 * Continuous timing loop that decrements all the system TTLs every kTTLDec seconds.
 *
 * THREADING: Runs in a detached thread.
 */
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
      peers.remove_if([&](PeerNode &p) { return p.has_drop_counter_expired(); });
      peerlist_lock.unlock();
   }
}