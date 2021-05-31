/*
 * PeerNode.cpp
 *
 * Source code for the PeerNode data structure, used to store peer data on the
 * Registration Server
 *
 *  Created on: May 25, 2021
 *      Author: smdupor
 */

#include "PeerNode.h"

//Constructor for a PeerNode which has not previously registered for the system
PeerNode::PeerNode(std::string hostname, int cookie, int port) {
	this -> hostname = hostname;
	this -> cookie = cookie;
	this -> port = port;
	activeNow = true;
	countActive = 0;
	timeReg =  std::time(nullptr);
	TTL=7200;
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

// Specialized tostring that returns the messaging-standardized format
std::string PeerNode::to_msg() {
	std::string value = "";
	if (activeNow){
		value = " " + hostname + " " + std::to_string(cookie) + " " + std::to_string(port)
				+ " " + " " + std::to_string(TTL) + " \n";
	}
	return value;
}

// Reset TTL when an active signal is received
void PeerNode::keepAlive() {
	TTL = 7200;
}

// Decrement TTL value when requested by controller
void PeerNode::decTTL() {
	TTL -= 30;
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
	if(this->cookie == cookie)
		return true;
	return false;
}

PeerNode::~PeerNode() {
	// TODO Auto-generated destructor stub
}

