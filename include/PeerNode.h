/*
 * PeerNode.h
 *
 *	A record for an individual peer, to be stored on the Registration Server.
 *
 *  Created on: May 25, 2021
 *      Author: smdupor
 */

#ifndef PEERNODE_H_
#define PEERNODE_H_

#include <ctime>
#include <string>
#include <cstring>

class PeerNode {
private:
	std::string hostname;
	int cookie; //unique id
	bool activeNow;
	int TTL;
	int port;
	int countActive;
	time_t timeReg;
	int dead_count;

public:
	PeerNode(std::string hostname, int cookie, int port); // Used on the registration server
   PeerNode(std::string hostname, int cookie, int port, int ttl); // Used on P2P Clients
	virtual ~PeerNode();
	std::string toS();
	std::string to_msg();
	void keepAlive();
	void dec_ttl();
	void decTTL(int seconds);
	void leave();
	void set_active(int ttl);
	void set_inactive();
	void report_down();
	void reset_down();

	// Getters
   bool active();
	bool equals(PeerNode *);
	bool equals(std::string);
	bool equals(int);
	std::string get_address();
	int get_port();

};

#endif /* PEERNODE_H_ */
