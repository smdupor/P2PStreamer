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

class RegistrationServer: public NetworkCommunicator  {
private:
	int latest_cookie;
	int accept_reg(sockinfo sock);
	std::string new_reg(std::vector<std::string> tokens, sockinfo sock);

public:
	RegistrationServer(std::string logfile, bool verbose_debug);
	virtual ~RegistrationServer();
	int start();
	void ttl_decrementer();
};

#endif /* REGISTRATIONSERVER_H_ */
