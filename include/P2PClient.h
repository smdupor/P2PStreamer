/**
 * P2PClient: Subclass of NetworkCommunicator
 *
 *	Contains the P2P client/server code for the file sharing nodes. Maintains the distributed index, serves files,
 *	and requests files to download from other peers.
 *
 *
 *  Created on: May 31, 2021
 *      Author: smdupor
 */

#ifndef INCLUDE_P2PCLIENT_H_
#define INCLUDE_P2PCLIENT_H_

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <list>
#include <ctime>
#include <algorithm>
#include <vector>
#include <thread>
#include <mutex>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#include "NetworkCommunicator.h"
#include "PeerNode.h"
#include "FileEntry.h"

/**
 * Encapsulate a datapoint for a time value when a qty of files have been downloaded.
 */
struct LogItem {
   // On creation, log the immediate (steady) time and the input quantity
   explicit LogItem(size_t qty) {
         this->qty = qty;
         this->time = std::chrono::steady_clock::now();
   }
   std::chrono::steady_clock::time_point time;
   size_t qty;
};


/**
 * P2P Client subclass header.
 */
class P2PClient : public NetworkCommunicator {
private:
   const char *reg_serv;
   std::string hostname, path_prefix;
   int ttl{}, cookie;
   size_t expected_qty{}, local_qty{}, system_wide_qty{}; // Number of files we want to download for this client, number stored locally
   std::list<FileEntry> files;
   std::list<LogItem> local_time_logs;
   long milliseconds_slept;

   // Inline these for performance optimization
   inline void contact_registration_server(int sockfd, bool registration);
   inline void parse_config(std::string config_file);
   inline void check_files();
   inline void transmit_file(int sockfd, FileEntry &file);
   inline void download_file(std::list<FileEntry>::iterator &want_file);

   void write_time_log();
   void downloader_backoff(size_t past_local_qty, int &backoff_time);
   void shutdown_system();
   void add_file_entry(const std::vector<std::string> &tokens);
   void find_wanted_file(std::_List_iterator<FileEntry> &want_file);
   void update_database(std::_List_iterator<FileEntry> &want_file);
   void ece_573_TA_interaction();
   void shuffle_peer_list();

public:
   P2PClient(std::string &addr_reg_server, std::string &logfile, bool verbose);
   ~P2PClient() override;
   void start(std::string config_file);
   void keep_alive();
   void accept_download_request(int sockfd);
   void downloader();
   bool get_system_on();
};

#endif /* INCLUDE_P2PCLIENT_H_ */
