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
#include <thread>
#include <mutex>


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
	bool reverse;

   int accept_incoming_request(sockinfo socket_data);
   std::string create_new_peer(sockinfo &socket_info);
   void handle_registration_request(sockinfo &socket_info, const std::vector<std::string> &tokens);
   void handle_leave_request(const sockinfo &socket_info, const std::vector<std::string> &tokens);
   void handle_keepalive_request(const sockinfo &socket_info, const std::vector<std::string> &tokens);
   void handle_get_peer_list_request(const sockinfo &socket_info, const std::vector<std::string> &tokens);

public:
	RegistrationServer(std::string logfile, bool verbose_debug);
	virtual ~RegistrationServer();
	void start();
	void ttl_decrementer();
};

#endif /* REGISTRATIONSERVER_H_ */
