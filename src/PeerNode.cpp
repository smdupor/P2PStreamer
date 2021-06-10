/*
 * PeerNode.cpp
 *
 * Source code for the PeerNode data structure, used to store peer data on the
 * Registration Server
 *
 *  Created on: May 25, 2021
 *      Author: smdupor
 */

#include <constants.h>
#include "PeerNode.h"

//Regserv
// Constructor for a PeerNode which has not previously registered for the system
PeerNode::PeerNode(std::string hostname, int cookie, int port) {
	this -> hostname = hostname;
	this -> cookie = cookie;
	this -> port = port;
	activeNow = true;
	reg_count = 1;
	timeReg =  std::time(nullptr);
	TTL=7200;
	dead_count = 0;
	lock_access = false;
}

//Constructor for PeerNodes that is used by the P2P clients system. registration time and count are not used, and set
//to null.
PeerNode::PeerNode(std::string hostname, int cookie, int port, int ttl) {
   this -> hostname = hostname;
   this -> cookie = cookie;
   this -> port = port;
   activeNow = true;
   TTL=ttl;
   dead_count = 0;
   timeReg = NULL;
   reg_count = NULL;
   lock_access = false;
}

//To_string functionality
std::string PeerNode::toS() {
	std::string value = "";
	value = "Hostname: " + hostname + " cookie: " + std::to_string(cookie) + " Port: " + std::to_string(port)
			+ " Time Registered: " + std::to_string(timeReg) + " TTL: " + std::to_string(TTL) + " ";
	if (activeNow)
		value += "Active: TRUE";
	else
		value += "Active: FALSE";
	return value;
}

/* Specialized tostring that returns the messaging-standardized format
 * Example: For Host bob.alice.com with cookie 432 on Port 1234 with ttl 300, will return:
 * " Host: bob.alice.com Cookie: 432 Port: 1234 TTL: 300 \n"
 * Tokenized:
 * [0] Control Port (NOT SET HERE, SET IN CALLER FUNCTION)
 * [1] Host:
 * [2] <hostname>
 * [3] Cookie:
 * [4] <cookie ID>
 * [5] Port:
 * [6] <port number>
 * [7] TTL:
 * [8] <TTL value>
 * [9] Active:
 * [10] TRUE
 * [11] <cr><lf>
 */
std::string PeerNode::to_msg() {
	std::string value = "";
	if (activeNow){
		value = " Host: " + hostname + " Cookie: " + std::to_string(cookie) + " Port: " + std::to_string(port)
				+ " TTL: " + std::to_string(TTL) + " Active: TRUE" + " \n";
	}
	else {
      value = " Host: " + hostname + " Cookie: " + std::to_string(cookie) + " Port: " + std::to_string(port)
              + " TTL: " + std::to_string(TTL) + " Active: FALSE" + " \n";
   }

	return value;
}

// Reset TTL when an active signal is received
void PeerNode::keepAlive() {
	TTL = 7200;
}

// Decrement TTL value when requested by controller
void PeerNode::dec_ttl() {
	TTL -= kTTLDec;
	if (TTL < 0)
	   TTL = 0;
}

void PeerNode::set_active(int ttl) {
   TTL = ttl;
   activeNow = true;

   // If host has actually timed out, make it inactive.
   if(TTL <= 0)
      set_inactive();
}

void PeerNode::set_inactive() {
   TTL = 0;
   activeNow = false;
}

// Test whether this host is active
bool PeerNode::active() {
	return activeNow;
}

// Deactivate host when host leaves the system
void PeerNode::leave() {
	activeNow = false;
	TTL = 0;
}

int PeerNode::get_cookie() {
   return cookie;
}

bool PeerNode::equals(PeerNode * other) {
	if(this->cookie != other->cookie)
		return false;
	if(strcmp(this->hostname.c_str(), other ->hostname.c_str()) != 0)
		return false;
	return true;
}

bool PeerNode::equals(std::string hostname) {
	if(strcmp(this->hostname.c_str(), hostname.c_str()) == 0)
		return true;
	return false;
}

bool PeerNode::equals(int cookie) {
	if(this->cookie == cookie && this->activeNow == true)
		return true;
	return false;
}

PeerNode::~PeerNode() {
	// TODO Auto-generated destructor stub
}

std::string PeerNode::get_address() {
   return hostname;
}

void PeerNode::increment_reg_count() {
   this->reg_count += 1;
}

void PeerNode::decTTL(int seconds) {
   TTL -= seconds;
   if (TTL <0)
      TTL = 0;
}

int PeerNode::get_port(){
   return port;
}

void PeerNode::report_down(){
   ++dead_count;
   if(dead_count > kTimeoutAttempts){
      activeNow = false;
      TTL=0;
   }
}

void PeerNode::reset_down() {
   dead_count = 0;
   activeNow = true;
}

bool PeerNode::locked(){
   return lock_access;
}

void PeerNode::lock() {
   lock_access = true;
}

void PeerNode::unlock() {
   lock_access = false;
}