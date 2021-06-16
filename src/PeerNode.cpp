/**
 * PeerNode
 *
 * Object containing the data for a remote network peer. Used by both clients and the registration server to manage
 * peer contact and ttl information
 *
 *  Created on: May 25, 2021
 *      Author: smdupor
 */

#include "PeerNode.h"

/**
 * Constructor intended for use on the Registration Server for a PeerNode which has not previously registered for the system
 */
PeerNode::PeerNode(std::string hostname, int cookie, int port) {
   this->hostname = hostname;
   this->cookie = cookie;
   this->port = port;
   activeNow = true;
   reg_count = 1;
   timeReg = std::time(nullptr);
   TTL = 7200;
   dead_count = 0;
   lock_access = false;
   ttl_drop_counter = 0;
}

/**
 * Constructor for PeerNodes that is used by the P2P client system. Registration time and count are not used at the
 * client end, so are permitted to be left null. Also, we will receive TTL data at creation from the RS, not make an
 * initialization decision ourselves.
 */
PeerNode::PeerNode(std::string hostname, int cookie, int port, int ttl) {
   this->hostname = hostname;
   this->cookie = cookie;
   this->port = port;
   activeNow = true;
   TTL = ttl;
   dead_count = 0;
   timeReg = -1;
   reg_count = -1;
   lock_access = false;
   ttl_drop_counter = 0;
}

/**
 * Destructor. Note that this is permitted to remain empty as we do not malloc() or 'new' anywhere, so default types
 * will be destructed on scope loss.
 */
PeerNode::~PeerNode() {

}

/**
 * Pretty to_string functionality for debugging
 */
std::string PeerNode::to_string() {
   std::string value = "";
   value = "Hostname: " + hostname + " cookie: " + std::to_string(cookie) + " Port: " + std::to_string(port)
           + " Time Registered: " + std::to_string(timeReg) + " TTL: " + std::to_string(TTL) + " ";
   if (activeNow)
      value += "Active: TRUE";
   else
      value += "Active: FALSE";
   return value;
}

/**
 * Specialized tostring that returns the messaging-standardized format string ready to be transmitted to a remote
 * host, less the control field (which is sent in the caller).
 */
std::string PeerNode::to_msg() {
   std::string message = " Cookie: " + std::to_string(cookie) + " Host: " + hostname + " Port: " + std::to_string(port)
                         + " TTL: " + std::to_string(TTL);
   if (activeNow) {
      message += " Active: TRUE \n";
   } else {
      message += " Active: FALSE \n";
   }
   return message;
}

/**
 * Getter for address
 */
std::string PeerNode::get_address() {
   return hostname;
}


/**
 * Getter for cookie
 */
int PeerNode::get_cookie() {
   return cookie;
}

/**
 * Reset the TTL value of this peer
 */
void PeerNode::keepAlive() {
   this->set_active(7200);
}

/**
 * Decrement the ttl value when requested by controller.
 */
void PeerNode::dec_ttl() {
   TTL -= kTTLDec;
   if (TTL < 0)
      TTL = 0;
}

/**
 * Mark this peer as active and reset the drop counter
 * @param ttl of the now-active host
 */
void PeerNode::set_active(int ttl) {
   this->TTL = ttl;
   this->activeNow = true;
   this->ttl_drop_counter = 0;
   // If host has actually timed out, make it inactive.
   if (this->TTL <= 0)
      set_inactive();
}

/**
 * Set this peer as active without modifying the TTL
 */
void PeerNode::set_active() {
   this->activeNow = true;
   if(this->TTL == 0)
      this->TTL = 7200;
}

/**
 * Mark this peer as inactive
 */
void PeerNode::set_inactive() {
   TTL = 0;
   activeNow = false;
}

/**
 * Note that another second has passed since this peer was marked as down or left the system
 */
void PeerNode::increment_drop_counter() {
   ++ttl_drop_counter;
}

/**
 * Once a host has left the system, mark as inactive.
 */
void PeerNode::leave() {
   activeNow = false;
   TTL = 0;
}

/**
 * Upon a returning registration, increment the quantity of times this peer has joined the system in the last 30 days.
 */
void PeerNode::increment_reg_count() {
   this->reg_count += 1;
}

/**
 * Reduce TTL by a specific, external quantity.
 * @param seconds to reduce ttl by.
 *
 */
void PeerNode::decTTL(int seconds) {
   TTL -= seconds;
   if (TTL < 0)
      TTL = 0;
}

/**
 * Getter for this peer's port
 */
int PeerNode::get_port() {
   return port;
}

/**
 * File report on this peer that another component had a failure or timeout condition on contact attempt, mark inactive,
 * and start the counter to determine if it should be dropped from the db.
 */
void PeerNode::report_down() {
   ++dead_count;
   if (dead_count > kTimeoutAttempts) {
      activeNow = false;
      TTL = 0;
   }
}

/**
 * Reset / clear the down report filed by report_down()
 */
void PeerNode::reset_down() {
   dead_count = 0;
   activeNow = true;
}

/**
 * Lock access to this specific peer
 */
void PeerNode::lock() {
   lock_access = true;
}

/**
 * Clear locked access to this specific peer
 */
void PeerNode::unlock() {
   lock_access = false;
}
/**
 * Test whether it's time to drop this peer from the database
 */
bool PeerNode::has_drop_counter_expired() {

   if (ttl_drop_counter > kTTLDec)
      return true;
   return false;
}

/**
 * Test whether this is a duplicate object
 */
bool PeerNode::equals(PeerNode *other) {
   if (this->cookie != other->cookie)
      return false;
   if (strcmp(this->hostname.c_str(), other->hostname.c_str()) != 0)
      return false;
   return true;
}

/**
 * Does this peer equal another based on hostname alone?
 */
bool PeerNode::equals(std::string hostname) {
   if (strcmp(this->hostname.c_str(), hostname.c_str()) == 0)
      return true;
   return false;
}

/**
 * Does this peer equal another based on cookie alone?
 */
bool PeerNode::equals(int cookie) {
   if (this->cookie == cookie)
      return true;
   return false;
}

/**
 * Test whether this host is marked active
 */
bool PeerNode::active() {
   return activeNow;
}

/**
 * Test whether this specific host is locked for access
 */
bool PeerNode::locked() {
   return lock_access;
}