/*
 * RegistrationServer.h
 *
 *	Class to encapsulate all top-level functionality of the registration server module
 *
 *  Created on: May 28, 2021
 *      Author: smdupor
 */

#ifndef REGISTRATIONSERVER_H_
#define REGISTRATIONSERVER_H_

#include <iostream>
#include <list>
#include <ctime>
#include <algorithm>
#include <cstring>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
 #include <arpa/inet.h>

#include "constants.h"
#include "PeerNode.h"
#include "Util.h"

// Hold information about a socket
struct sockinfo {
		int socket;
		char * cli_addr;
	};

class RegistrationServer {
private:
	std::list <PeerNode> peers;
	std::string log;
	int latest_cookie;
	bool lock;
	int port;
	bool debug;
	std::time_t start_time;
	int accept_reg(sockinfo sock);
	std::string new_reg(std::vector<std::string> tokens, sockinfo sock);
	void ttl_decrementer();
	void verbose(std::string output);

public:
	RegistrationServer(std::string logfile, bool verbose_debug);
	virtual ~RegistrationServer();
	int start();
};

#endif /* REGISTRATIONSERVER_H_ */
