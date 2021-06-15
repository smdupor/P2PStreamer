/**
 * PeerNode
 *
 * Object containing the data for a remote network peer. Used by both clients and the registration server to manage
 * peer contact and ttl information
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
	int cookie, TTL, port, reg_count, dead_count, ttl_drop_counter;
	bool activeNow;
	time_t timeReg;
   const int kTTLDec = 7; // TTLs shall be decremented every this many seconds//////IMPORTANT: ALSO Dec'l in NetworkCommunicator.h
   const int kTimeoutAttempts = 5; // This is the number of retries before a host is declared down.//////IMPORTANT: ALSO Dec'l in NetworkCommunicator.h
   bool lock_access;

public:
	PeerNode(std::string hostname, int cookie, int port); // Used on the registration server
   PeerNode(std::string hostname, int cookie, int port, int ttl); // Used on P2P Clients
	virtual ~PeerNode();

	//Getters and to_strings
	std::string to_string();
	std::string to_msg();
   int get_port();
   int get_cookie();
   std::string get_address();

   //Setters
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
   void increment_drop_counter();

	// Booleans
   bool active();
	bool equals(PeerNode *);
	bool equals(std::string);
	bool equals(int);
	bool has_drop_counter_expired();
};

#endif /* PEERNODE_H_ */
