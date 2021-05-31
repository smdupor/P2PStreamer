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
	PeerNode(std::string hostname, int cookie, int port);
	virtual ~PeerNode();
	std::string toS();
	std::string to_msg();
	void keepAlive();
	void decTTL();
	bool active();
	void leave();
	bool equals(PeerNode *);
	bool equals(std::string);
	bool equals(int);
};

#endif /* PEERNODE_H_ */
