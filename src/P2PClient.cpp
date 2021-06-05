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
	cookie = -1;
}

P2PClient::~P2PClient() {
	// TODO Auto-generated destructor stub
}

// Boot up the P2P client and make first registration
void P2PClient::start(std::string config_file) {
   // Connect to the Registration Server and Register
   int sockfd = outgoing_connection(reg_serv, kControlPort);
   get_peer_list(sockfd, true);

   // Load our configuration file to set up the local files in our distributed DB
   parse_config(config_file);

}

void P2PClient::get_peer_list(int sockfd, bool registration) {
   // Initialize control variables and message strings
   bool loop_control = true;
   std::string in_message, out_message;

   if(registration && cookie == -1){ // This is a new registration
      out_message = kCliRegister + " NEW \n";
      transmit(sockfd, out_message);
   } else if (registration) { // This is a returning registration
      ///////////////////////////TODO Returning Registration//////////////////////////
   }
   else { //This client is already registered and we are requesting the peer list
      out_message = kGetPeerList + " " + "Cookie: " + std::to_string(cookie)+" \n";
      transmit(sockfd, out_message);
   }


   while (loop_control) {
      // Get back response
      in_message = receive(sockfd);
      std::vector<std::string> messages = split((const std::string &) in_message, '\n');
      // Split buffer into individual messages
      for (size_t i=0; i< messages.size(); ++i) {
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
         }
         else {
           usleep(1000); // Buffer was empty; Sleep for 1ms
           ////////////////////////////////////////TODO//////////////////////////////////// May need timeout
         }
      } //for
   } // while(loop_control)
}


void P2PClient::keep_alive() {
   while(1){
      std::cout << "sleeping for 10 in keepalive";
      std::this_thread::sleep_for(std::chrono::seconds(10));
      for(PeerNode p : peers){
         p.dec_ttl();
      }
    //  int sockfd = outgoing_connection(reg_serv);
    //  std::string outgoing_message = kKeepAlive + " " + std::to_string(cookie) + " \n";
     // close(sockfd);
   }
}

int P2PClient::outgoing_connection(std::string hostname, int port) {
   struct sockaddr_in serv_addr;
   struct hostent *server;

  // int port = kControlPort;
   int sockfd;

   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) {
      verbose("ERROR opening socket");
      return -1;
   }

   server = gethostbyname(hostname.c_str());

   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

   serv_addr.sin_port = htons(port);
   if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      verbose("ERROR connecting to remote socket");
      return -1;
   }
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

int P2PClient::listener() {
   int sockfd; // socket descriptor
   socklen_t clilen; //client length
   struct sockaddr_in serv_addr, cli_addr; //socket addresses
   sockinfo accepted_socket; // Values passed on once a connection is accepted

   // Create the socket
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd<0){
      verbose("ERROR opening socket");
      accepted_socket.socket = -1;
      return -1;
   }

   // Initialize address and port values
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(port);

   // Bind the socket
   if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
      verbose("Error on socket bind");
      return -1;
   }

   // Listen for new connections
   verbose("LISTENING FOR CONNECTIONS on port: " + std::to_string(port));
   listen(sockfd,10);
   clilen = sizeof(cli_addr);
/*
   // Loop continuously to accept new connections
   while(1){
      //accept the connection
      accepted_socket.socket = (int) accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

      // Handle the incoming request
      //threads.emplace_back(new std::thread((accept_download_request), accepted_socket.socket));
      //std::thread( [this] { accept_download_request(accepted_socket); } );

      if (accepted_socket.socket < 0){
         verbose("Error on accepting connection");
         return;
      }
   }
   */
return sockfd;
}

void P2PClient::accept_download_request(int sockfd){
   //std::cout << "Starting accept\n";
   //sleep(1);
   //std::cout << "Continuing accept\n";
   std::string temp =  receive(sockfd);
   close(sockfd);
}

void P2PClient::downloader(){
   std::string remote_addr, outgoing_message;
   int remote_port, sockfd;
   int i=0;

   sleep(2);

   while (this->expected_qty > i){


      ///////////////for now, just contact the first one////////////////////
      for (PeerNode &p : peers){
         //std::cout << "The remote peer list says: " << p.toS();
         if(p.active()){
            remote_addr = p.get_address();
            remote_port = p.get_port();
            sockfd = outgoing_connection(remote_addr.c_str(), remote_port);
            if(sockfd > 0) {
               outgoing_message =
                       "This message is only a test from " + hostname + " on port " + std::to_string(port) + " randseed: " +
                       std::to_string(rand() % 100) + "\n";
               transmit(sockfd, outgoing_message);
               close(sockfd);
            } else {
               p.report_down();
            }
         }
      }
      ++i;
      //std::cout<<"Sleeping for 2 in downloader\n";
      sleep(2);
      sockfd = outgoing_connection(reg_serv, kControlPort);
      get_peer_list(sockfd, false);
      close(sockfd);
      if(i%12 == 0){
         debug_print_hosts_and_files();
      }
   }
}