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
	system_on = true;
}

P2PClient::~P2PClient() {
	// TODO Auto-generated destructor stub
}

bool P2PClient::get_system_on(){
   return system_on;
}

// Boot up the P2P client and make first registration
void P2PClient::start(std::string config_file) {
   // Connect to the Registration Server and Register
   int sockfd = outgoing_connection(reg_serv, kControlPort);
   get_peer_list(sockfd, true);

   // Load our configuration file to set up the local files in our distributed DB
   parse_config(config_file);
   check_files();
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
      while(!in_message.empty() && in_message.substr(in_message.length()-1)!="\n"){
         in_message += receive(sockfd);
      }
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
                     /////////////For now, we are going to prefer our own recognizance that the host is indeed down.
                  //p->set_active(stoi(tokens[8]));
               } else { // We have it, but it's inactive according to the registration server
                  p->set_inactive();
               }
            } //if(cookie != me)
         } // else if(tokens[0] == kPeerListItem
         else if (tokens[0] == kDone) { // List is finished downloading
            close(sockfd);
            loop_control = false;
         }
         else {
           usleep(kEmptyBufferSleep); // Buffer was empty; Sleep for kEmptyBufferSleep microseconds.
           ////////////////////////////////////////TODO//////////////////////////////////// May need timeout
         }
      } //for
   } // while(loop_control)
}

void P2PClient::keep_alive() {
   int sockfd;
   while(system_on){
      //std::cout << "sleeping for 10 in keepalive";
      std::this_thread::sleep_for(std::chrono::seconds(kKeepAliveTimeout));
      sockfd = outgoing_connection(reg_serv, kControlPort);
      std::string outgoing_message = kKeepAlive + " " + std::to_string(cookie) + " \n";
      close(sockfd);
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

      // How many files will be available across the system (which will trigger a clean shutdown)
      bzero(buffer,512);
      file.getline(buffer,512);
      tokens = split(std::string(buffer), '=');
      this->system_wide_qty = stoi(tokens[1]);

      // Get the prefix for the download directory
      bzero(buffer,512);
      file.getline(buffer,512);
      tokens = split(std::string(buffer), '=');
      path_prefix = std::string(tokens[1]);

      // Get each filename that we want to share and have available locally
      for(int i=0; i < local_qty; ++i){
         bzero(buffer,512);
         file.getline(buffer,512);
         local_file = path_prefix + std::string(buffer);
         files.push_back(FileEntry(stoi(std::string(buffer).substr(3,4)), hostname, cookie,
                                   local_file, true));
      }
   }
   else{
      std::cout << "PROBLEM WITH CONFIGURATION FILE.";
   }
   file.close();
}

void P2PClient::check_files(){
   for (FileEntry &file : files) {
      std::ifstream infile(file.get_path());
      // Use C-Style IO to write data to the socket.
      char buffer[1024];
      int bytecount=0;
      int n=0;
      bzero(buffer,1024);
      while(infile.getline(buffer,1024)){
         buffer[strlen(buffer)] ='\n';
         bytecount += strlen(buffer);
         bzero(buffer,1024);
      }
      infile.close();
      file.set_length(bytecount);
      file.clear_lock();
      verbose("Checked File: " + file.get_path() + " Length: " + std::to_string(bytecount));
   }
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
return sockfd;
}

void P2PClient::accept_download_request(int sockfd){
   std::string remote_addr, out_message, in_message;
   std::vector<std::string> messages, tokens;

   bool loop_control = true;

   // Start by passing along the distributed database
   while (loop_control) {
      in_message = receive(sockfd, "Line 258");
      /*if(in_message.length() >= kDone.length()){
      error("Substringing:'"+in_message.substr(in_message.length()-kDone.length()-3)+"'\n");
      while(in_message != kDone + " \n" && in_message.substr(in_message.length()-kDone.length()-3) != kDone + " \n"){
         in_message += receive(sockfd);
      }}*/
      while(in_message.length() != 0 && in_message.substr(in_message.length()-1)!="\n"){
         in_message += receive(sockfd, "Line265");
      }
      std::vector<std::string> messages = split((const std::string &) in_message, '\n');
      // Split buffer into individual messages
      for (std::string &message : messages) {
         // Split into tokens
         std::vector<std::string> tokens = split(message, ' ');

         // Client is requesting the index
         if (tokens[0] == kGetIndex) {

            int temp_cookie = stoi(tokens[2]);
            for(FileEntry &file : files){
               out_message = kIndexItem + file.to_msg();
               transmit(sockfd, out_message);
            }
            out_message = kDone + " \n";
            transmit(sockfd, out_message);
            loop_control = false;
         }
         else if (tokens[0] == kGetFile) {
            //return " FileID: " + std::to_string(id) + " Cookie: " + std::to_string(cookie) + " Hostname: " + hostname +  " \n";
            // [2] file id, [4] cookie, [6] hostname
            auto want_file = std::find_if(files.begin(), files.end(), [&](FileEntry& f) {
               return f.equals(stoi(tokens[2]), this->cookie); });

            std::ifstream infile(want_file->get_path());
            out_message = kFileLine + " Length: " + std::to_string(want_file->get_length()) + " ";
            transmit(sockfd, out_message);
/*
            verbose("I'm about to send: " + want_file->to_s());
            sleep(1);
            if(infile){
               verbose("The file handle appears to be good?");
            }*/
            // Use C-Style IO to write data to the socket.
            char buffer[1024];
            int n=0;
            bzero(buffer,1024);
            while(infile.getline(buffer,1024)){
                               buffer[strlen(buffer)] = '\n';
                  n = write(sockfd, buffer, strlen(buffer));
                  if (n < 0)
                     error("ERROR writing file data to socket");

                  //print_sent(std::string(buffer));
                  bzero(buffer, 1024);

            }
            print_sent("Just sent:" + want_file->to_s() + " \n");

            infile.close();
            want_file->clear_lock();
            //debug_print_hosts_and_files();

            loop_control = false;
         }
         else {
            usleep(kEmptyBufferSleep); // Buffer was empty; Sleep for kEmptyBufferSleep microseconds
         }
      } //for
   } // while(loop_control)
}

void transmit_file(int sockfd, FileEntry &file);

void P2PClient::downloader() {
   std::string remote_addr, outgoing_message, incoming_message;
   std::vector<std::string> messages, tokens;
   std::list<FileEntry>::iterator want_file;

   int remote_port, sockfd;
   int temp_id, temp_cookie, past_local_qty, slowdown = 1;
   bool picked_file = false;

   // Run Downloads as long as we don't have all the files we want
   while (expected_qty > local_qty) {
      past_local_qty = local_qty;
      // Contact the registration server and get the list of peers
      sockfd = outgoing_connection(reg_serv, kControlPort);
      get_peer_list(sockfd, false);
      close(sockfd);

      // Contact all the peers and get their distributed databases
      for (PeerNode &p : peers) {
         if (p.active()) {
            sockfd = outgoing_connection(p.get_address().c_str(), p.get_port());
            if (sockfd > 0) {
               outgoing_message = kGetIndex + " Cookie: " + std::to_string(cookie) + " \n";
               transmit(sockfd, outgoing_message);
               bool done = false;
               while (!done) {
                  incoming_message = receive(sockfd, "Line355");
                  while (incoming_message.length() != 0 &&
                         incoming_message.substr(incoming_message.length() - 1) != "\n") {
                     incoming_message += receive(sockfd, "Line357");
                  }
                  messages = split(incoming_message, '\n');
                  for (std::string &message : messages) {
                     tokens = split(message, ' ');
                     /////////////////TODO Define tokens[] in commments////////////////
                     // If it's a file index item, and not one of my local ones, check if we have it and add if not.
                     if (tokens[0] == kIndexItem) {
                        //&& stoi(tokens[4]) != cookie
                        // check to see if we have it
                        auto file = std::find_if(files.begin(), files.end(), [&](FileEntry &file) {
                           return file.equals(stoi(tokens[2]), stoi(tokens[4])); // tokens[2] = id, tokens[4] = cookie
                        });

                        // We don't have it, need to add it
                        if (file == files.end()) {
                           /////////////////TODO Define tokens[] in commments////////////////
                           std::string temp_path = path_prefix + "rfc" + tokens[2] + ".txt";
                           int temp_id = stoi(tokens[2]);
                           int temp_cookie = stoi(tokens[4]);

                           bool temp_local = false;

                           // if we can find A copy of the fileentry that is local, then we will note in the DB
                           // that this file is also stored locally on this machine
                           for (FileEntry &temp : files) {
                              if (temp.equals(temp_id) && temp.is_local()) {
                                 temp_local = true;
                              }
                           }

                           // If someone else is modifying the (whole) list, wait until it's unlocked
                           //while(lock=true) { usleep(rand() % 100); } // If someone else is modifying the list, wait

                           lock = true;
                           // Add to the database. tokens[6] contains the hostname.
                           files.push_back(FileEntry(temp_id, tokens[6],
                                                     temp_cookie, temp_path, temp_local));
                           lock = false;
                        }
                     } else if (tokens[0] == kDone) {
                        done = true;
                        close(sockfd);
                     } else if (tokens[0] == "") {

                     } else {
                        error("There was a problem with the TCP message received by downloader(): Buffer holds:" +
                              tokens[0]);
                     }
                  }
               }
            } else { p.report_down(); }
         }
      }

      ///////////////////////TODO: Drop inactive peers from the distributed database. Maybe do this in  the hadnler????///////////////

      //////////////////////TODO: Pick a file, and ask to download it //////////////////////////////

      download_file();
      // We have not been able to get anything new, back off the download process
      if (past_local_qty == local_qty) {
         if(slowdown < 5000)
            slowdown *= 2;
         std::this_thread::sleep_for(std::chrono::milliseconds(slowdown));
         verbose("Nothing new available to download. Waiting for: " + std::to_string(((float) slowdown) * 0.001) + " Seconds.");
       } else {
         slowdown = 1;
      }

   }
}
// Pick a file that we want to get

// For now, let's pick the first file in the database that we don't have. Possibly, in the future,
// performance might be enhanced by a more random picking
   void P2PClient::download_file() {
   std::string incoming_message, outgoing_message;
   int sockfd;
   std::vector<std::string> tokens;

   auto want_file = std::find_if(this->files.begin(), this->files.end(), [](FileEntry &f) {
      return !f.is_local() && !f.is_locked();
   });
   if (want_file != this->files.end()) {
      auto peer = std::find_if(this->peers.begin(), this->peers.end(), [&](PeerNode &node) {
         return node.equals(want_file->get_cookie());
      });
      // ask for the file
      sockfd = this->outgoing_connection(peer->get_address(), peer->get_port());
      outgoing_message = kGetFile + want_file->to_msg();
      this->transmit(sockfd, outgoing_message);
      // open the file

      int retry = 0;
      do {
         //get the control packet that tells us the lengths
         incoming_message = this->receive(sockfd, "Line442");
         tokens = this->split(incoming_message, ' ');
         std::this_thread::sleep_for(std::chrono::milliseconds(50));
         ++retry;
      } while (incoming_message.length() == 0 && retry < 15);

      if (retry != 15) {
         std::ofstream output_file(want_file->get_path());
         //////////////////////////////////////////////////////////////////////////////////////////////////////TODO: Set up parsing of the control packet properly.
         int end_length = stoi(tokens[2]);
         int bytes_written = 0;
         if (tokens.size() > 3) {
            int initial_offset = tokens[0].length() + tokens[1].length() + tokens[2].length() + 3;
            incoming_message = incoming_message.substr(initial_offset);
            output_file.write(incoming_message.c_str(), incoming_message.length());
            bytes_written += incoming_message.length();
         }

         while (bytes_written < end_length) {
            incoming_message = this->receive(sockfd, "Line457");
            output_file.write(incoming_message.c_str(), incoming_message.length());
            bytes_written += incoming_message.length();
         }
         // Close the file and the connection.
         output_file.close();
         want_file->clear_lock();
         close(sockfd);
         // Add the file to the database
         this->files.push_back(FileEntry(want_file->get_id(), this->hostname, this->cookie, want_file->get_path()));

         // Update our quantities
         ++this->local_qty;
         this->print_recv("I now have this many files: " + std::to_string(this->local_qty));
         // Check through the database and mark any entries of this file as also-locally-available.
         for (FileEntry &f : this->files) {
            if (f.get_id() == want_file->get_id()) {
               f.set_local();
            }
         }
      } else {
         // mark host as dead
         peer->set_inactive();
      }

   } else {
      usleep(kEmptyBufferSleep); // Nothing available to download right now; Sleep and retry
   }
}