/*
 * NetworkCommunicator.h
 *
 * Superclass to hold all the shared functions and variables between client and server
 *
 *  Created on: May 31, 2021
 *      Author: smdupor
 */

#ifndef INCLUDE_NETWORKCOMMUNICATOR_H_
#define INCLUDE_NETWORKCOMMUNICATOR_H_

#include <iostream>
#include <list>
#include <vector>

#include <string.h>
#include <unistd.h>

#include "PeerNode.h"

// Hold information about a socket
struct sockinfo {
		int socket;
		char * cli_addr;
	};

class NetworkCommunicator {

protected:
   // Declare networking constants
   const std::string kDone = "P2PDI:0.1/DONE"; // Message indicates all data for this section of program flow is done
   const int kInitialTTL = 7200;
   const int kTTLDec = 7; // TTLs shall be decremented every this many seconds//////IMPORTANT: ALSO Dec'l in PeerNode.h
   const int MSG_LEN = 1024; // Size that all message buffers are initialized to
   const int kKeepAliveTimeout = 20; // Seconds between keepalive pings
   const int kEmptyBufferSleep = 1000; // Quantity of microseconds the system should sleep for when it expects more msgs. Default to 1millisecond.
   const int kTimeoutRetry = 10000; // After ten seconds, assume we will not receive from the client.

// Registration server constants and methods
   const int kControlPort = 65432; // The port listening on the RS
   const int kFileKeepAliveTimeout = 30; //Interval on which we decrement file database TTLs
   const std::string kCliRegister = "P2PDI:0.1/CLIREG"; // Client requests to register to the RS
   const std::string kCliRegAck = "P2PDI:0.1/CLIACK"; // Response acking this client registration and providing port, cookie
   const std::string kPeerListItem = "P2PDI:0.1/PEERITEM"; // Response from RS signaling message contains data for one (active) Peer
   const std::string kGetPeerList = "P2PDI:0.1/GETPEERLST"; // Registered client requesting the updated peer list
   const std::string kKeepAlive = "P2PDI:0.1/KEEPALIVE"; // Message indicating this client is still alive
   const std::string kLeave = "P2PDI:0.1/LEAVE"; // Client indicating they are leaving the system

   // P2P Client constants and control methods
   const std::string kGetIndex = "P2PDI:0.1/GETDILIST"; // Request from a peer asking for this peer's copy of the Distributed index
   const std::string kIndexItem = "P2PDI:0.1/LISTITEM"; // Response to a peer containing a member of the distributed index on this host
   const std::string kGetFile = "P2PDI:0.1/GETFILE"; // Request from a peer for a file located on this host
   const std::string kFileLine = "P2PDI:0.1/DATA"; // Response from a peer indicating this message will contain a line of the file being transmitted

   // Indices for value lookups in tokenized (split) messages.
   enum {CONTROL = 0, COOKIE = 2, HOSTNAME = 4, FILEID = 6, TTL = 8, PORT = 6, ACTIVE = 10};

	std::list <PeerNode> peers;
	std::string log;
	bool lock;
	int port;
	bool debug, system_on;
	std::time_t start_time;

	virtual void ttl_decrementer();
	std::vector<std::string> split(const std::string &input, char delim);
   void transmit(int sockfd, std::string &out_message);
   std::string receive(int sockfd);
   int outgoing_connection(std::string hostname, int port);
   std::string receive_no_delim(int sockfd);
   void print_sent(std::string input);
   void print_recv(std::string input);
   void verbose(std::string input);

public:
	virtual ~NetworkCommunicator();
   int listener(int listen_port);
   int get_port();
   static void error(std::string input);
   static void warning(std::string input);
   static void info(std::string input);
};

#endif /* INCLUDE_NETWORKCOMMUNICATOR_H_ */
