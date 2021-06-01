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
#include <vector>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "NetworkCommunicator.h"

class P2PClient : public NetworkCommunicator {
private:
	const char * reg_serv;

public:
	P2PClient(std::string addr_reg_server, std::string logfile, bool verbose);
	virtual ~P2PClient();
	int start();
};

#endif /* INCLUDE_P2PCLIENT_H_ */
