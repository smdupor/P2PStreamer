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

class P2PClient {
private:


public:
	P2PClient();
	virtual ~P2PClient();
	int start();
};

#endif /* INCLUDE_P2PCLIENT_H_ */
