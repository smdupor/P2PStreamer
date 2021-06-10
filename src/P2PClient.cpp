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
   // Reset registration keepalive counter
   timeout_counter = 0;

   if(registration && cookie == -1){ // This is a new registration
      out_message = kCliRegister + " NEW \n\n";
      transmit(sockfd, out_message);
   }
   /*else if (registration) { // This is a returning registration
      ///////////////////////////TODO Returning Registration//////////////////////////
   }*/
   else { //This client is already registered and we are requesting the peer list
      out_message = kGetPeerList + " " + "Cookie: " + std::to_string(cookie)+ "\n\n";
      transmit(sockfd, out_message);
   }

      // Get back response
      in_message = receive(sockfd);
      std::vector<std::string> messages = split((const std::string &) in_message, '\n');
      // Split buffer into individual messages
      for (std::string &message : messages) {
         // Split into tokens
         std::vector<std::string> tokens = split((const std::string &) message, ' ');

         if (tokens[0] == kCliRegAck) { // Welcome to system, here is your ID info
            hostname = tokens[2];
            cookie = stoi(tokens[4]);
            this->port = stoi(tokens[6]);
            this->ttl = stoi(tokens[8]);
         }
         else if (tokens[0] == kPeerListItem) { // We have been sent a peer list node item
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
                  p->set_active(stoi(tokens[8]));
               } else if(tokens[10] == "FALSE") { // We have it, and it's inactive
                  p->set_inactive();
                  // Drop its files from the database
                  files.remove_if([&](FileEntry f) {return f.equals(*p);});
               }
               else { // We have it, but it's inactive according to the registration server
                  p->set_inactive();
               }
            } //if(cookie != me)
         } // else if(tokens[0] == kPeerListItem
         else if (tokens[0] == kDone) { // List is finished downloading
            close(sockfd);
            loop_control = false;
         }
      }
   }


void P2PClient::keep_alive() {
   int sockfd, timeout_counter;

   while(system_on){
      std::this_thread::sleep_for(std::chrono::seconds(1));
			if(timeout_counter >= kKeepAliveTimeout) {
      sockfd = outgoing_connection(reg_serv, kControlPort);
			if (sockfd > 0) {
      std::string outgoing_message = kKeepAlive + " Cookie: " + std::to_string(cookie) + " \n\n";
			std::string incoming_message = receive(sockfd); // swallow the ack
      close(sockfd);
			timeout_counter = 0;
		}
		}

   }
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
      verbose(std::string(buffer));
      this->expected_qty = stoi(tokens[1]);

      // How many files are stored locally
      bzero(buffer,512);
      file.getline(buffer,512);
      verbose(std::string(buffer));
      tokens = split(std::string(buffer), '=');
      this->local_qty = stoi(tokens[1]);

      // How many files will be available across the system (which will trigger a clean shutdown)
      bzero(buffer,512);
      file.getline(buffer,512);
      verbose(std::string(buffer));
      tokens = split(std::string(buffer), '=');
      this->system_wide_qty = stoi(tokens[1]);

      // Get the prefix for the download directory
      bzero(buffer,512);
      file.getline(buffer,512);
      verbose(std::string(buffer));
      tokens = split(std::string(buffer), '=');
      path_prefix = std::string(tokens[1]);

      // Get each filename that we want to share and have available locally
      for(int i=0; i < local_qty; ++i){
         bzero(buffer,512);
         file.getline(buffer, 512);
         verbose(std::string(buffer));
         local_file = std::string(path_prefix + std::string(buffer));
        // error(local_file);
         files.push_back(FileEntry(stoi(std::string(buffer).substr(3,4)), hostname, cookie,
                                   local_file, true));
      }
      for(FileEntry &f : files){
         verbose(f.to_s());}
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



void P2PClient::accept_download_request(int sockfd){
   std::string remote_addr, out_message, in_message;
   std::vector<std::string> messages, tokens;

   bool loop_control = true;

   // Start by passing along the distributed database
      in_message = receive(sockfd);

      // If we got a timeout condition
      if(in_message.length() == 0){
         close(sockfd);
         error("***********RETURNING********************");
         return;

      }


      messages = split((const std::string &) in_message, '\n');
      // Split buffer into individual messages
      for (std::string &message : messages) {
         // Split into tokens
         tokens = split(message, ' ');

         // Client is requesting the index
         if (tokens[0] == kGetIndex) {
            int temp_cookie = stoi(tokens[2]);
            for(FileEntry &file : files){
               out_message = kIndexItem + file.to_msg();
               transmit(sockfd, out_message);
            }
            out_message = kDone + " \n\n";
            transmit(sockfd, out_message);
            loop_control = false;
         }
         else if (tokens[0] == kGetFile) {
            //return " FileID: " + std::to_string(id) + " Cookie: " + std::to_string(cookie) + " Hostname: " + hostname +  " \n";
            // [2] file id, [4] cookie, [6] hostname
            auto want_file = std::find_if(files.begin(), files.end(), [&](FileEntry& f) {
               return f.equals(stoi(tokens[2]), this->cookie); });

            transmit_file(sockfd,  *want_file);
         }
      }
}

void P2PClient::transmit_file(int sockfd, FileEntry &want_file) {
	std::string out_message;
   std::ifstream infile(want_file.get_path());
	out_message = kFileLine + " Length: " + std::to_string(want_file.get_length()) + " "+"\n";
	transmit(sockfd, out_message);
   //std::this_thread::sleep_for(std::chrono::seconds(2));
	// Use C-Style IO to write data to the socket.
	char buffer[MSG_LEN*2];
	int n=0;
	bzero(buffer,MSG_LEN*2);
	while(infile.getline(buffer,MSG_LEN*2)){
			// Add back the newline stripped by getline()
				buffer[strlen(buffer)] = '\n';
				transmit(sockfd,std::string(buffer));
            std::this_thread::sleep_for(std::chrono::microseconds(500));
				// Write the line to the socket
			//	n = write(sockfd, buffer, strlen(buffer));
				//if (n < 0)
					// error("ERROR writing file data to socket");
			//	print_sent(std::string(buffer));
				bzero(buffer, MSG_LEN*2);
	}
	print_sent("Just sent:" + want_file.to_s() + " \n");

	infile.close();
	want_file.clear_lock();
	//debug_print_hosts_and_files();
}

void P2PClient::downloader() {
   std::string remote_addr, outgoing_message, incoming_message;
   std::vector<std::string> messages, tokens;
   std::list<FileEntry>::iterator want_file;

   int remote_port;
   int temp_id, temp_cookie, past_local_qty, slowdown = 1;
   bool picked_file = false;

   // Run Downloads as long as we don't have all the files we want
   while (system_on) {
      past_local_qty = local_qty;
      // Contact the registration server and get the list of peers
     int sockfd = outgoing_connection(reg_serv, kControlPort);
      get_peer_list(sockfd, false);
      close(sockfd);

      // Contact all the peers and get their distributed databases
      for (PeerNode &p : peers) {
         if (p.active() && !p.locked()) {
           int sockfd = outgoing_connection(p.get_address(), p.get_port());
            if (sockfd > 0) {
               outgoing_message = kGetIndex + " Cookie: " + std::to_string(cookie) + " \n\n";
               transmit(sockfd, outgoing_message);
               bool done = false;
               /*while (!done) {

                  while (incoming_message.length() != 0 &&
                         incoming_message.substr(incoming_message.length() - 1) != "\n") {
                     incoming_message += receive(sockfd, "Line357");
                  }*/
               incoming_message = receive(sockfd);
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
                        error("blank token");
                     } else {
                        error("There was a problem with the TCP message received by downloader(): Buffer holds:" +
                              tokens[0]);
                     }
                  }
                  close(sockfd);
               }
            } else { p.report_down(); }
         } //for

         //download_file();

      // Pick a file, and ask to download it
      auto want_file = std::find_if(files.begin(), files.end(), [](FileEntry &f) {
         return !f.is_local() && !f.is_locked();
      });
      if (want_file != files.end()) {
         PeerNode &peer = *std::find_if(peers.begin(), peers.end(), [&](PeerNode &node) {
            return node.equals(want_file->get_cookie());
         });
         peer.lock();

         // ask for the file
         sockfd = outgoing_connection(peer.get_address(), peer.get_port());
         outgoing_message = kGetFile + want_file->to_msg()+"\n";

         transmit(sockfd, outgoing_message);

         //get the control packet that tells us the lengths
         incoming_message = receive_no_delim(sockfd);
         messages = split(incoming_message, '\n');
         if(messages[0].length() > 0) {
            tokens = split(messages[0], ' ');

            std::ofstream output_file(want_file->get_path());
            int end_length = stoi(tokens[2]);
            int bytes_written = 0;

            // If we have gotten both the header and some data (likely) split and use the data
            if (messages.size() > 1) {
               int initial_offset = messages[0].length() + 1; // tokens[0].length() + tokens[1].length() + tokens[2].length() + 3;
               incoming_message = incoming_message.substr(initial_offset);
               output_file.write(incoming_message.c_str(), incoming_message.length());
               bytes_written += incoming_message.length();
            }

            // Continue to get file data and write to file until we have the entire file
            while (bytes_written < end_length) {
               incoming_message = receive_no_delim(sockfd);
               output_file.write(incoming_message.c_str(), incoming_message.length());
              // error(incoming_message);
               bytes_written += incoming_message.length();
            }
            // Close the file and the connection.
            output_file.close();
            want_file->clear_lock();
            close(sockfd);
            peer.unlock();
            // Add the file to the database
            files.push_back(FileEntry(want_file->get_id(), hostname, cookie, want_file->get_path()));

            // Update our quantities
            ++local_qty;
            //print_recv("I now have this many files: " + std::to_string(local_qty));
            // Check through the database and mark any entries of this file as also-locally-available.
            for (FileEntry &f : files) {
               if (f.get_id() == want_file->get_id()) {
                  f.set_local();
               }
            }
         } else {
            error("An error occurred during the download step; Likely, a malformed packet was received.");
         }
      }

      // We have not been able to get anything new, back off the download process
      if (past_local_qty == local_qty) {
         if(slowdown < 5000)
            slowdown *= 2;
         std::this_thread::sleep_for(std::chrono::milliseconds(slowdown));
         verbose("Nothing new available to download. Waiting for: " + std::to_string(((float) slowdown) * 0.001) + " Seconds.");
       } else {
         slowdown = 1;
      }
      error("SyswideQty " + std::to_string(system_wide_qty) + " DB SIze:" + std::to_string(files.size()) +
             " \n");
      if(system_wide_qty == files.size()) {
         error("SyswideQty Reached+ " + std::to_string(system_wide_qty) + " " + std::to_string(files.size()) +
               " Exiting Download Loop \n");
         std::this_thread::sleep_for(std::chrono::seconds (3));
         sockfd = outgoing_connection(reg_serv, kControlPort);
         outgoing_message = kLeave + " Cookie: " + std::to_string(cookie) + " \n\n";
         transmit_no_throttle(sockfd, outgoing_message);
         system_on=false;
         incoming_message = receive(sockfd);
         close(sockfd);
      }

   }
}


// Pick a file that we want to get

// For now, let's pick the first file in the database that we don't have. Possibly, in the future,
// performance might be enhanced by a more random picking
   void P2PClient::download_file() {
   std::string incoming_message, outgoing_message;
   int sockfd;
   std::vector<std::string> tokens, messages;

   auto want_file = std::find_if(files.begin(), files.end(), [](FileEntry &f) {
      return !f.is_local() && !f.is_locked();
   });
   if (want_file != files.end()) {
      PeerNode &peer = *std::find_if(peers.begin(), peers.end(), [&](PeerNode &node) {
         return node.equals(want_file->get_cookie());
      });
      peer.lock();
      debug_print_hosts_and_files();
      // ask for the file
      sockfd = outgoing_connection(peer.get_address(), peer.get_port());
      outgoing_message = kGetFile + want_file->to_msg()+"\n";

      transmit(sockfd, outgoing_message);

         //get the control packet that tells us the lengths
         incoming_message = receive_no_delim(sockfd);
         messages = split(incoming_message, '\n');
         if(messages[0].length() > 0) {
            tokens = split(messages[0], ' ');

            std::ofstream output_file(want_file->get_path());
            //////////////////////////////////////////////////////////////////////////////////////////////////////TODO: Set up parsing of the control packet properly.
            int end_length = stoi(tokens[2]);
            int bytes_written = 0;

            // Implies that we have accidentally gotten more than the header. Makes more sense to read the header, stop, then read the data.
            if (messages.size() > 1) {
               int initial_offset = messages[0].length() + 1; // tokens[0].length() + tokens[1].length() + tokens[2].length() + 3;
               incoming_message = incoming_message.substr(initial_offset);
               output_file.write(incoming_message.c_str(), incoming_message.length());
               bytes_written += incoming_message.length();
            }

            while (bytes_written < end_length) {
               incoming_message = receive_no_delim(sockfd);
               output_file.write(incoming_message.c_str(), incoming_message.length());
               //error(incoming_message);
               bytes_written += incoming_message.length();
            }
            // Close the file and the connection.
            output_file.close();
            want_file->clear_lock();
            close(sockfd);
            peer.unlock();
            // Add the file to the database
            files.push_back(FileEntry(want_file->get_id(), hostname, cookie, want_file->get_path()));

            // Update our quantities
            ++local_qty;
            print_recv("I now have this many files: " + std::to_string(local_qty) + "\n");
            // Check through the database and mark any entries of this file as also-locally-available.
            for (FileEntry &f : files) {
               if (f.get_id() == want_file->get_id()) {
                  f.set_local();
               }
            }
            debug_print_hosts_and_files();
         } else {
            error("An error occurred during the download step; Likely, a malformed packet was received.");
         }
   }
}
