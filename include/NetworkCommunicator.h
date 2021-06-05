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
   void print_sent(std::string input);
   void print_recv(std::string input);
   void error(std::string input);
   void verbose(std::string input);

public:
	virtual ~NetworkCommunicator();
};

#endif /* INCLUDE_NETWORKCOMMUNICATOR_H_ */
