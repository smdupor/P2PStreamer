/*
 * P2PClient.cpp
 *
 *	Contains top-level client code for the P2P Client hosts
 *
 *  Created on: May 31, 2021
 *      Author: smdupor
 */

#include "P2PClient.h"

P2PClient::P2PClient(std::string addr_reg_serv, std::string logfile, bool verbose) {
	reg_serv = (const char *) addr_reg_serv.c_str();
	log = logfile;
	lock = false;
	debug = verbose;
	start_time = (const time_t) std::time(nullptr);
}

P2PClient::~P2PClient() {
	// TODO Auto-generated destructor stub
}

// Boot up the P2P client and make first registration
int P2PClient::start() {
   struct sockaddr_in serv_addr;
   struct hostent *server;

   int port = kControlPort;
   int sockfd;

   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0)
      verbose("ERROR opening socket");

   server = gethostbyname(reg_serv);

   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

   serv_addr.sin_port = htons(port);
   if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
      verbose("ERROR connecting to remote socket");

   register_new(sockfd);

   return sockfd;
}

void P2PClient::register_new(int sockfd) {

   // Initialize control variables and message strings
   int n;
   bool loop_control = true;
   std::string in_message, out_message;

   // Send a new registration message
   out_message = kCliRegister + " NEW \n";
   transmit(sockfd, out_message);

   while (loop_control) {
      // Get back response
      in_message = receive(sockfd);
      std::vector<std::string> messages = split((const std::string &) in_message, '\n');
      // Split buffer into individual messages
      for (int i=0; i< messages.size(); ++i) {
         // Split into tokens
         std::vector<std::string> tokens = split((const std::string &) messages[i], ' ');

         if (tokens[0] == kCliRegAck) { // Welcome to system, here is your ID info
            hostname = tokens[2];
            cookie = stoi(tokens[4]);
            port = stoi(tokens[6]);
            ttl = stoi(tokens[8]);
         } else if (tokens[0] == kPeerListItem) { // We have been sent a peer list node item
            // Check to ensure this item is not me
            if (cookie != stoi(tokens[4])) {
               // check to see if we have it
               auto p = std::find_if(peers.begin(), peers.end(), [&](PeerNode node) {
                  return node.equals(stoi(tokens[4]));
               });
               if (p == peers.end() && tokens[10] == "TRUE") { // We don't have it, and it's an active host
                  lock = true;
                  peers.push_back(PeerNode(tokens[2], stoi(tokens[4]), stoi(tokens[6]), stoi(tokens[8])));
                  lock = false;
               } else if (tokens[10] == "TRUE") { // We have it, and it's active
                  p->set_active(stoi(tokens[8]));
               } else { // We have it, but it's inactive
                  p->set_inactive();
               }
            } //if(cookie != me)
         } // else if(tokens[0] == kPeerListItem
         else if (tokens[0] == kDone) { // List is finished downloading
            close(sockfd);
            loop_control = false;
         } else {
           usleep(1000); // Buffer was empty; Sleep for 1ms
           ////////////////////////////////////////TODO//////////////////////////////////// May need timeout
         }
      } // while(loop_control)
   }
}

void P2PClient::get_peer_list() {

}

void P2PClient::keep_alive() {

}
