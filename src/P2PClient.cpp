/*
 * P2PClient.cpp
 *
 *	Contains top-level client code for the P2P Client hosts
 *
 *  Created on: May 31, 2021
 *      Author: smdupor
 */

#include "P2PClient.h"

P2PClient::P2PClient(std::string addr_reg_serv, std::string logfile, bool verbose) {
	reg_serv = (const char *) addr_reg_serv.c_str();
	log = logfile;
	lock = false;
	debug = verbose;
	start_time = (const time_t) std::time(nullptr);
}

P2PClient::~P2PClient() {
	// TODO Auto-generated destructor stub
}

