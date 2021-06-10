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
	int reg_count;
	time_t timeReg;
	int dead_count;
   const int kTTLDec = 7; // TTLs shall be decremented every this many seconds//////IMPORTANT: ALSO Dec'l in NetworkCommunicator.h
   const int kTimeoutAttempts = 5; // This is the number of retries before a host is declared down.//////IMPORTANT: ALSO Dec'l in NetworkCommunicator.h
   bool lock_access;

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
	void increment_reg_count();
	bool locked();
	void lock();
	void unlock();

	// Getters
   bool active();
	bool equals(PeerNode *);
	bool equals(std::string);
	bool equals(int);
	std::string get_address();
	int get_port();
	int get_cookie();


};

#endif /* PEERNODE_H_ */
