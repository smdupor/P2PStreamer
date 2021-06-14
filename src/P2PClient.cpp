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

std::mutex downloader_lock, files_lock;

P2PClient::P2PClient(std::string &addr_reg_serv, std::string &logfile, bool verbose) {
	reg_serv = (const char *) addr_reg_serv.c_str();
	log = logfile;
	lock = false;
	debug = verbose;
	start_time = (const time_t) std::time(nullptr);
	cookie = -1;
	system_on = true;
}

P2PClient::~P2PClient() {

}

bool P2PClient::get_system_on(){
   return system_on;
}

// Boot up the P2P client and make first registration
void P2PClient::start(std::string config_file) {
   // Connect to the Registration Server and Register
   int sockfd = outgoing_connection(reg_serv, kControlPort);

   get_peer_list(sockfd, true);
   close(sockfd);
   // Load our configuration file to set up the local files in our distributed DB
   parse_config(config_file);
   check_files();
   logs.push_back(LogItem(local_qty));
}

inline void P2PClient::get_peer_list(int sockfd, bool registration) {
   // Initialize control variables and message strings
   downloader_lock.lock();
   std::string in_message, out_message;
   // Reset registration keepalive counter

   if(registration && cookie == -1){ // This is a new registration
      out_message = kCliRegister + " NEW \n\n";
      transmit(sockfd, out_message);
   }
   else if (registration) {
      out_message = kCliRegister + " Cookie: " + std::to_string(cookie) + " \n\n";
      transmit(sockfd, out_message);
   }
   else { //This client is already registered and we are requesting the peer list
      out_message = kGetPeerList + " " + "Cookie: " + std::to_string(cookie)+ " \n\n";
      transmit(sockfd, out_message);
   }

      // Get back response
      in_message = receive(sockfd);
      std::vector<std::string> messages = split((const std::string &) in_message, '\n');
      // Split buffer into individual messages
      for (std::string &message : messages) {
         // Split into tokens
         std::vector<std::string> tokens = split((const std::string &) message, ' ');
         if (tokens[CONTROL] == kCliRegAck) { // Welcome to system, here is your ID info
            hostname = tokens[HOSTNAME];
            cookie = stoi(tokens[COOKIE]);
            this->port = stoi(tokens[PORT]);
            this->ttl = stoi(tokens[TTL]);
         }
         else if (tokens[CONTROL] == kPeerListItem) { // We have been sent a peer list node item
            // Check to ensure this item is not me
            if (cookie != stoi(tokens[COOKIE])) {
               // check to see if we have it
               auto p = std::find_if(peers.begin(), peers.end(), [&](PeerNode node) {
                  return node.equals(stoi(tokens[COOKIE]));
               });
               if (p == peers.end() && tokens[ACTIVE] == "TRUE") { // We don't have it, and it's an active host

                  peers.push_back(PeerNode(tokens[HOSTNAME], stoi(tokens[COOKIE]), stoi(tokens[PORT]), stoi(tokens[TTL])));
                  lock = false;
               } else if (tokens[ACTIVE] == "TRUE") { // We have it, and it's active
                  p->set_active(stoi(tokens[TTL]));
               } else if(tokens[ACTIVE] == "FALSE") { // We have it, and it's inactive
                  p->set_inactive();
                  // Drop its files from the database
                  files.remove_if([&](FileEntry f) {return f.equals(*p);});
               }
            }
         }
         else if (tokens[CONTROL] == kDone) { // List is finished downloading
            downloader_lock.unlock();
            return;
         }
      }
}

void P2PClient::keep_alive() {
   int client_ttl_counter=0, file_ttl_counter=0;

   while(system_on){
      std::this_thread::sleep_for(std::chrono::seconds(1));
      ++client_ttl_counter;
      ++file_ttl_counter;
      if(client_ttl_counter >= kKeepAliveTimeout) {
         int sockfd = outgoing_connection(reg_serv, kControlPort);
         if (sockfd > 0) {
            std::string outgoing_message = kKeepAlive + " Cookie: " + std::to_string(cookie) + " \n\n";
            std::string incoming_message = receive(sockfd); // swallow the ack
            close(sockfd);
            client_ttl_counter = 0;
         }
      }
      if(file_ttl_counter >= kFileKeepAliveTimeout) {
         for(FileEntry &f : files){
            if(f.get_hostname() != this->hostname)
               f.decrement_ttl();
         }
         file_ttl_counter = 0;
      }
   }
}

inline void P2PClient::parse_config(std::string config_file) {
   downloader_lock.lock();
   files_lock.lock();
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
      for(size_t i=0; i < local_qty; ++i){
         bzero(buffer,512);
         file.getline(buffer, 512);
         local_file = std::string(path_prefix + std::string(buffer));
         files.push_back(FileEntry(stoi(std::string(buffer).substr(3,4)), hostname, cookie,
                                   local_file, true, kInitialTTL));
      }
   }
   else{
      std::cout << "*****************************PROBLEM WITH CONFIGURATION FILE.***************************";
      std::cout << "The system has likely been started from an incorrect working directory. ";
      system_on = false;
   }
   file.close();
}

inline void P2PClient::check_files(){
   for (FileEntry &file : files) {
      std::ifstream infile(file.get_path());
      char buffer[MSG_LEN];
      int bytecount=0;
      bzero(buffer,MSG_LEN);
      while(infile.getline(buffer,MSG_LEN)){
         buffer[strlen(buffer)] ='\n';
         bytecount += strlen(buffer);
         bzero(buffer,MSG_LEN);
      }
      infile.close();
      file.set_length(bytecount);
      file.clear_lock();
    }
   downloader_lock.unlock();
   files_lock.unlock();
}

void P2PClient::accept_download_request(int sockfd) {
   std::string remote_addr, out_message, in_message;
   std::vector<std::string> messages, tokens;

   in_message = receive(sockfd);

   messages = split((const std::string &) in_message, '\n');
   // Split buffer into individual messages
   for (std::string &message : messages) {
      // Split into tokens
      tokens = split(message, ' ');

      // Client is requesting the index
      if (tokens[CONTROL] == kGetIndex) {
         for (FileEntry file : files) {
            out_message = kIndexItem + file.to_msg();
            transmit(sockfd, out_message);
         }
         out_message = kDone + " \n\n";
         transmit(sockfd, out_message);

         std::this_thread::sleep_for(std::chrono::seconds(2));

         return;
      } else if (tokens[CONTROL] == kGetFile) {
         auto want_file = *std::find_if(files.begin(), files.end(), [&](FileEntry &f) {
            return f.equals(stoi(tokens[FILEID]), this->cookie);
         });

         transmit_file(sockfd, want_file);
      }
   }
}

inline void P2PClient::transmit_file(int sockfd, FileEntry &want_file) {
	std::string out_message;
   std::ifstream infile(want_file.get_path());

   out_message = kFileLine + " Length: " + std::to_string(want_file.get_length()) + " "+"\n";
   transmit(sockfd, out_message);

   char buffer[MSG_LEN*2];
   bzero(buffer,MSG_LEN*2);

   while(infile.getline(buffer,MSG_LEN*2)){
      // Add back the newline stripped by getline()
      buffer[strlen(buffer)] = '\n';
      out_message = std::string(buffer);
      transmit(sockfd,out_message);
      bzero(buffer, MSG_LEN*2);
   }
   infile.close();
   want_file.clear_lock();

}


void P2PClient::downloader() {
   std::string remote_addr, outgoing_message, incoming_message;
   std::vector<std::string> messages, tokens;
   std::list<FileEntry>::iterator want_file;
   size_t past_local_qty;
   int backoff_time = 10;

   // Run Downloads as long as we don't have all the files we want
   while (system_on) {
      past_local_qty = local_qty;
      // Contact the registration server and get the list of peers
     int sockfd = outgoing_connection(reg_serv, kControlPort);

   if (sockfd >= 0) {
      get_peer_list(sockfd, false);
      close(sockfd);
      downloader_lock.lock();

      // Round robin the peer list to spread load
     /* if (rand() % 10 > 4) {
         peers.reverse();
      }*/

      auto want_file = files.begin();
      find_wanted_file(want_file);

      if(want_file == files.end()) {
         want_file = update_database(want_file);
      }
      // Find the first file in our list that we want.

      download_file(want_file);
      downloader_lock.unlock();
      downloader_backoff(past_local_qty, backoff_time);

      if (system_wide_qty == files.size()) {
         shutdown_system();
      }
   }
   }
}

std::_List_iterator<FileEntry> &P2PClient::update_database(std::_List_iterator<FileEntry> &want_file) {
   std::string outgoing_message, incoming_message;
   std::vector<std::string> messages, tokens;

   for(int i=0; i < 3; i++) {
      std::list<PeerNode> shuffled_peers;
      for (PeerNode &p : peers) {
         if (rand() % 2 == 1) {
            shuffled_peers.push_front(p);
         } else {
            shuffled_peers.push_back(p);
         }
      }
      std::swap(shuffled_peers, peers);
   }

   for (PeerNode &p : peers) {
      if (p.active() && !p.locked()) {
         int sockfd = outgoing_connection(p.get_address(), p.get_port());

         if (sockfd >= 0) {
            outgoing_message = kGetIndex + " Cookie: " + std::to_string(cookie) + " \n\n";
            transmit(sockfd, outgoing_message);

            incoming_message = receive(sockfd);
            messages = split(incoming_message, '\n');

            for (std::string &message : messages) {
               tokens = split(message, ' ');
               // If it's a file index item, and not one of my local ones, check if we have it and add if not.
               if (tokens[CONTROL] == kIndexItem) {

                  // check to see if we have it
                  auto file = std::find_if(files.begin(), files.end(), [&](FileEntry &f) {
                     return f.equals(stoi(tokens[FILEID]), stoi(tokens[COOKIE]));
                  });

                  // We don't have it, need to add it
                  if (file == files.end()) {
                     add_file_entry(tokens);
                  }
               } else if (tokens[CONTROL] == kDone) {
                  close(sockfd);
               } else {
                  error("There was a problem with the TCP message received by downloader(): Buffer holds:" +
                        tokens[CONTROL]);
               }
            }
         }
      } else {
         error("DEAD SOCK 383\n");
         p.report_down();
      }

      find_wanted_file(want_file);
      if (want_file != files.end()){
         break;
        }
   } //for
   return want_file;
}

void P2PClient::find_wanted_file(std::_List_iterator<FileEntry> &want_file) {
   want_file = std::find_if(files.begin(), files.end(), [&](FileEntry &f) {
            return !f.is_local() && !f.is_locked();
         });
   int count_non_local = 0;
   for(FileEntry &f : files) {
      if(!f.is_local()) {
         ++count_non_local;
      }
   }
   if(count_non_local == 0){
      want_file = files.end();
   } else {
      static constexpr double fraction { 1.0 / (RAND_MAX + 1.0) };
      int rand_selector = 1 + static_cast<int>((count_non_local) * (std::rand() * fraction));
      std::_List_iterator<FileEntry> temp = files.begin();
      for(int i = 0; i<rand_selector; i++) {
          temp = std::find_if(temp, files.end(), [&](FileEntry &f) {
            return !f.is_local() && !f.is_locked();});
      }
      want_file = temp;
   }
}

void P2PClient::add_file_entry(const std::vector<std::string> &tokens) {
   std::string temp_path = path_prefix + "rfc" + tokens[FILEID] + ".txt";
   int temp_id = stoi(tokens[FILEID]);
   int temp_cookie = stoi(tokens[COOKIE]);

   bool temp_local = false;

   // if we can find A copy of the fileentry that is local, then we will note in the DB
// that this file is also stored locally on this machine
   for (FileEntry &temp : files) {
      if (temp.equals(temp_id) && temp.is_local()) {
         temp_local = true;
      }
   }

   files.push_back(FileEntry(temp_id, tokens[HOSTNAME],
                             temp_cookie, temp_path, temp_local, stoi(tokens[TTL])));
}

void P2PClient::shutdown_system() {
   verbose("SyswideQty Reached:  " + std::to_string(system_wide_qty) + " " + std::to_string(files.size()) +
         " Exiting Download Loop \n");
   std::this_thread::sleep_for(std::chrono::seconds(3));

   write_time_log();

   int sockfd = outgoing_connection(reg_serv, kControlPort);
   std::string outgoing_message = kLeave + " Cookie: " + std::to_string(cookie) + " \n\n";
   transmit(sockfd, outgoing_message);
   system_on = false;
   std::this_thread::sleep_for(std::chrono::milliseconds(500));
   //std::string incoming_message = receive(sockfd);
   close(sockfd);
}

void P2PClient::downloader_backoff(size_t past_local_qty, int &backoff_time) {// We have not been able to get anything new, back off the download process
   if (past_local_qty == local_qty) {
      if (backoff_time < 1000) {
         backoff_time *= 2;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(backoff_time));
      verbose("Nothing new available to download. Waiting for: " + std::to_string(((float) backoff_time) * 0.001) +
              " Seconds.");
   } else {
      backoff_time = 10;
   }
   error("SyswideQty " + std::to_string(system_wide_qty) + " DB SIze:" + std::to_string(files.size()) +
         " \n");
}

void P2PClient::write_time_log() {// Dump the logfile
   std::string outgoing_message;

   LogItem t = *logs.begin();
   std::ofstream dmp(log);
   outgoing_message = "Qty, t\n";
   verbose(outgoing_message);
   dmp.write(outgoing_message.c_str(), outgoing_message.length());

   for (LogItem &l : logs) {
      outgoing_message = std::to_string(l.qty) + ", " +
                         std::to_string(((float) std::chrono::duration_cast<std::chrono::milliseconds>(
                                 l.time - t.time).count()) / 1000) +
                         "\n";
      dmp.write(outgoing_message.c_str(), outgoing_message.length());
      verbose(outgoing_message);
   }
   dmp.close();
}

inline void P2PClient::download_file(std::list<FileEntry>::iterator &want_file) {
   std::string incoming_message, outgoing_message;
   std::vector<std::string> tokens, messages;

   if (want_file != files.end()) {
      PeerNode &peer = *std::find_if(peers.begin(), peers.end(), [&](PeerNode &node) {
         return node.equals(want_file->get_cookie());
      });
      peer.lock();

      // ask for the file
      int sockfd = outgoing_connection(peer.get_address(), peer.get_port());
      outgoing_message = kGetFile + want_file->to_msg() + "\n";

      transmit(sockfd, outgoing_message);

      //get the control packet that tells us the lengths
      incoming_message = receive_no_delim(sockfd);
      messages = split(incoming_message, '\n');
      if (messages[0].length() > 0) {
         tokens = split(messages[0], ' ');

         std::ofstream output_file(want_file->get_path());
         int end_length = stoi(tokens[2]);
         int bytes_written = 0;

         // If we have gotten both the header and some data (likely) split and use the data
         if (messages.size() > 1) {
            int initial_offset = messages[0].length() + 1;
            incoming_message = incoming_message.substr(initial_offset);
            output_file.write(incoming_message.c_str(), incoming_message.length());
            bytes_written += incoming_message.length();
         }

         // Continue to get file data and write to file until we have the entire file
         while (bytes_written < end_length) {
            incoming_message = receive_no_delim(sockfd);
            output_file.write(incoming_message.c_str(), incoming_message.length());
            bytes_written += incoming_message.length();
         }
         // Close the file and the connection.
         output_file.close();
         want_file->clear_lock();
         close(sockfd);
         peer.unlock();
         // Add the file to the database
         files.push_back(FileEntry(want_file->get_id(), hostname, cookie, want_file->get_path(), end_length));

         // Update our quantities
         ++local_qty;
         logs.push_back(LogItem(local_qty));

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
}
