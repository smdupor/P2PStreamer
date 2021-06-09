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

#include "constants.h"
#include "PeerNode.h"

// Hold information about a socket
struct sockinfo {
		int socket;
		char * cli_addr;
	};

class NetworkCommunicator {

protected:

   // Declare networking constants
   const std::string kDone = "DONE"; // Message indicates all data for this section of program flow is done
   const int kTTLDec = 7; // TTLs shall be decremented every this many seconds//////IMPORTANT: ALSO Dec'l in PeerNode.h
   const int MSG_LEN = 1024; // Size that all message buffers are initialized to
   const int kTimeoutAttempts = 5; // This is the number of retries before a host is declared down.//////IMPORTANT: ALSO Dec'l in PeerNode.h
   const int kKeepAliveTimeout = 120; // Seconds between keepalive pings
   const int kEmptyBufferSleep = 1000; // Quantity of microseconds the system should sleep for when it expects more msgs. Default to 1millisecond.
	 const int kTimeoutRetry = 10000; // After ten seconds, assume we will not receive from the client.

// Registration server constants and methods
   const int kControlPort = 65432; // The port listening on the RS
   const std::string kCliRegister = "CREG"; // Client requests to register to the RS
   const std::string kCliRegAck = "CACK"; // Response acking this client registration and providing port, cookie
   const std::string kPeerListItem = "PEER"; // Response from RS signaling message contains data for one (active) Peer
   const std::string kGetPeerList = "PLST"; // Registered client requesting the updated peer list
   const std::string kKeepAlive = "ALIV"; // Message indicating this client is still alive
   const std::string kLeave = "LEAV"; // Client indicating they are leaving the system

// P2P Client constants and control methods
   const std::string kGetIndex = "LIST"; // Request from a peer asking for this peer's copy of the Distributed index
   const std::string kIndexItem = "LSTI"; // Response to a peer containing a member of the distributed index on this host
   const std::string kGetFile = "GETF"; // Request from a peer for a file located on this host
   const std::string kNackFile = "NCKF"; // Response from a peer that this file isn't available from this peer right now (Locked by another transmission)
   const std::string kSendingFile = "SEND"; // Response from a peer confirming that the peer will commence transmitting the file
   const std::string kFileLine = "DATA"; // Response from a peer indicating this message will contain a line of the file being transmitted

	std::list <PeerNode> peers;
	std::string log;
	bool lock;
	int port;
	bool debug;
	std::time_t start_time;

	virtual void ttl_decrementer();
	std::vector<std::string> split(const std::string &input, char delim);
   void transmit(int sockfd, std::string &out_message);
   std::string receive(int sockfd);
	 int outgoing_connection(std::string hostname, int port);
   std::string receive(int sockfd, std::string debug);
   //char *receive_cstr(int sockfd);
   void print_sent(std::string input);
   void print_recv(std::string input);
   void error(std::string input);
   void verbose(std::string input);

public:
	virtual ~NetworkCommunicator();
};

#endif /* INCLUDE_NETWORKCOMMUNICATOR_H_ */
