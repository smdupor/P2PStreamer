/*
 * P2PClient.cpp
 *
 *	Contains top-level client code for the P2P Client hosts
 *
 *  Created on: May 31, 2021
 *      Author: smdupor
 */

#include "NetworkCommunicator.h"
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
void P2PClient::start(std::string config_file) {
   // Connect to the Registration Server and Register
   int sockfd = outgoing_connection(reg_serv);
   register_new(sockfd);

   // Load our configuration file to set up the local files in our distributed DB
   parse_config(config_file);
}

void P2PClient::register_new(int sockfd) {
   // Initialize control variables and message strings
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
      for (size_t i=0; i< messages.size(); ++i) {
         // Split into tokens
         std::vector<std::string> tokens = split((const std::string &) messages[i], ' ');
         /*for(size_t t=0;t<tokens.size();++t){
            std::cout << "i= " << i << " t="<< t << ": "<<tokens[t]<<"\n";
         }*/

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
         }
         else {
           usleep(1000); // Buffer was empty; Sleep for 1ms
           ////////////////////////////////////////TODO//////////////////////////////////// May need timeout
         }
      } //for
   } // while(loop_control)
}

void P2PClient::get_peer_list() {

}

void P2PClient::keep_alive() {

}

int P2PClient::outgoing_connection(std::string hostname) {
   struct sockaddr_in serv_addr;
   struct hostent *server;

   int port = kControlPort;
   int sockfd;

   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0)
      verbose("ERROR opening socket");

   server = gethostbyname(hostname.c_str());

   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

   serv_addr.sin_port = htons(port);
   if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
      verbose("ERROR connecting to remote socket");
   return sockfd;
}

void P2PClient::parse_config(std::string config_file) {
   char buffer[512];
   std::vector<std::string> tokens;
   std::string prefix;
   std::string local_file;


   std::ifstream file(config_file.c_str());

   if(file)
   {
      // How many files we want to download
      bzero(buffer,512);
      file.getline(buffer,512);
      tokens = split(std::string(buffer), '=');
      this->expected_qty = stoi(tokens[1]);

      // How many files are stored locally
      bzero(buffer,512);
      file.getline(buffer,512);
      tokens = split(std::string(buffer), '=');
      this->local_qty = stoi(tokens[1]);

      // Get the prefix for the download directory
      bzero(buffer,512);
      file.getline(buffer,512);
      tokens = split(std::string(buffer), '=');
      prefix = std::string(tokens[1]);

      // Get each filename that we want to share
      for(int i=1; i < local_qty; ++i){
         bzero(buffer,512);
         file.getline(buffer,512);
         local_file = prefix + std::string(buffer);
         files.push_back(FileEntry(stoi(std::string(buffer).substr(3,4)), hostname, cookie,
                                   local_file, true));
      }
   }
   else{
      std::cout << "PROBLEM WITH CONFIGURATION FILE.";
   }
   file.close();
}

void P2PClient::debug_print_hosts_and_files(){
   std::cout << "\n\n\nYou requested a system report. Here it is. Note, Only OTHER peers will show up here.: \n";
   for(PeerNode p : peers) {
      std::cout << "Peer Data: " << p.toS()<<"\n";
   }
   for(FileEntry f : files) {
      std::cout << "File Data: " << f.to_s()<<"\n";
   }
}