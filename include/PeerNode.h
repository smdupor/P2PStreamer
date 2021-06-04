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

public:
	PeerNode(std::string hostname, int cookie, int port); // Used on the registration server
   PeerNode(std::string hostname, int cookie, int port, int ttl); // Used on P2P Clients
	virtual ~PeerNode();
	std::string toS();
	std::string to_msg();
	void keepAlive();
	void decTTL();
	void leave();
	void set_active(int ttl);
	void set_inactive();

	// Getters
   bool active();
	bool equals(PeerNode *);
	bool equals(std::string);
	bool equals(int);
};

#endif /* PEERNODE_H_ */
