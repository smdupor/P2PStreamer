/*
 * P2PClient.h
 *
 *	Contains top-level client code for the P2P Client hosts
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
#include "constants.h"
#include "PeerNode.h"
#include "FileEntry.h"

struct LogItem {
   LogItem(int qty) {
         this->qty = qty;
         this->time = std::chrono::steady_clock::now();
         //#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
         //#define timeNow() std::chrono::high_resolution_clock::now()
   }
   std::chrono::steady_clock::time_point time;
   int qty;
};



class P2PClient : public NetworkCommunicator {
private:
   const char *reg_serv;
   std::string hostname, path_prefix;
   int ttl, cookie, timeout_counter;
   int expected_qty, local_qty, system_wide_qty; // Number of files we want to download for this client, number stored locally
   std::list<FileEntry> files;
   bool system_on;
   std::list<LogItem> logs;
   int logging_offset;


   inline void get_peer_list(int sockfd, bool registration);

   //int outgoing_connection(std::string hostname, int port);
   inline void parse_config(std::string config_file);
   inline void check_files();
   inline void transmit_file(int sockfd, FileEntry &file);



public:
   P2PClient(std::string addr_reg_server, std::string logfile, bool verbose);
   ~P2PClient() override;
   void start(std::string config_file);
   void keep_alive();
   //int listener();
   //void *downloader(void *thread_id);
   void accept_download_request(int sockfd);
   void debug_print_hosts_and_files();
   void downloader();
   inline void download_file(std::list<FileEntry>::iterator &want_file);
   bool get_system_on();
};

#endif /* INCLUDE_P2PCLIENT_H_ */
