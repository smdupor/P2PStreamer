/**
 * NetworkCommunicator
 *
 * The NetworkCommunicator superclass encapsulates all functionality that is shared by both major components of the system,
 * the Registration Server and the P2P Client. This includes establishing listening sockets, establishing outgoing
 * connections, transmitting strings of data on the TCP stream, and receiving strings of data back. Any universally-shared
 * instance variables (such as port, a list of peers, verbosity flags) are also encapsulated in this superclass.
 *
 * Also, all of the messaging constants, and system-wide numeric constants (like timeout settings) are encapsulated here.
 *
 * Finally, universally shared utility methods, such as a string splitter and multiple pretty-print methods, are
 * encapsulated here.
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
   const int kInitialTTL = 7200; // quantity TTL shall be set to initiall in seconds.
   const int kTTLDec = 5; // TTLs shall be decremented every this many seconds//////IMPORTANT: ALSO Dec'l in PeerNode.h
   const int MSG_LEN = 1024; // Size that all message buffers are initialized to
   const int kKeepAliveTimeout = 20; // Seconds between keepalive pings
   const int kEmptyBufferSleep = 1000; // Quantity of microseconds the system should sleep for when it expects more msgs. Default to 1millisecond.
   const int kTimeoutRetry = 10000; // After ten seconds, assume we will not receive from the client.

// Registration server constants and methods
   const int kControlPort = 65432; // The port listening on the RS
   const int kFileKeepAliveTimeout = 30; //Interval on which we decrement file database TTLs
   const std::string kCliRegister = "P2PDI:0.1/CLI_REGISTER"; // Client requests to register to the RS
   const std::string kCliRegAck = "P2PDI:0.1/CLI_ACK"; // Response acking this client registration and providing port, cookie
   const std::string kPeerListItem = "P2PDI:0.1/PEER_ITEM"; // Response from RS signaling message contains data for one (active) Peer
   const std::string kGetPeerList = "P2PDI:0.1/P_QUERY"; // Registered client requesting the updated peer list
   const std::string kKeepAlive = "P2PDI:0.1/KEEPALIVE"; // Message indicating this client is still alive
   const std::string kLeave = "P2PDI:0.1/LEAVE"; // Client indicating they are leaving the system

   // P2P Client constants and control methods
   const std::string kGetIndex = "P2PDI:0.1/RFC_QUERY"; // Request from a peer asking for this peer's copy of the Distributed index
   const std::string kIndexItem = "P2PDI:0.1/RFC_ITEM"; // Response to a peer containing a member of the distributed index on this host
   const std::string kGetFile = "P2PDI:0.1/GET_RFC"; // Request from a peer for a file located on this host
   const std::string kFileLine = "P2PDI:0.1/RFC_DATA"; // Response from a peer indicating this message will contain a line of the file being transmitted

   // Indices for value lookups in tokenized (split) messages.
   enum {CONTROL = 0, COOKIE = 2, HOSTNAME = 4, FILEID = 6, TTL = 8, PORT = 6, ACTIVE = 10};

	std::list <PeerNode> peers;
	std::string log;
	bool lock;
	int port;
	bool debug, system_on;
	std::time_t start_time;

	// Socket communication
   void transmit(int sockfd, std::string &out_message);
   std::string receive(int sockfd);
   int outgoing_connection(std::string hostname, int port);
   std::string receive_no_delim(int sockfd);

   // Utility methods
   virtual void ttl_decrementer();
   std::vector<std::string> split(const std::string &input, char delim);
   void print_sent(std::string input);
   void print_recv(std::string input);
   void verbose(std::string input);

public:
	virtual ~NetworkCommunicator();
   int listener(int listen_port);
   int get_port();

   //Externally-accessible print methods (used in int main()s)
   static void error(std::string input);
   static void warning(std::string input);
   static void info(std::string input);
};

#endif /* INCLUDE_NETWORKCOMMUNICATOR_H_ */
