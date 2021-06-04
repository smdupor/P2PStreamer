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

class P2PClient : public NetworkCommunicator {
private:
   const char *reg_serv;
   std::string hostname;
   int ttl, cookie, port;
   int expected_qty, local_qty; // Number of files we want to download for this client, number stored locally
   std::list<FileEntry> files;

   void register_new(int sockfd);
   void get_peer_list();
   void keep_alive();
   int outgoing_connection(std::string hostname);

   void parse_config(std::string config_file);

public:
   P2PClient(std::string addr_reg_server, std::string logfile, bool verbose);
   ~P2PClient() override;
   void start(std::string config_file);
   void debug_print_hosts_and_files();

};

#endif /* INCLUDE_P2PCLIENT_H_ */
