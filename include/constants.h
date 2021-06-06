/*
 * constants.h
 *
 *	Contains all constant ints (eg port numbers) as well as all control code constants used
 *	when building and interpreting messages platform-wide
 *
 *  Created on: May 25, 2021
 *      Author: smdupor
 */

#ifndef P2P_INCLUDE_CONSTANTS_H_
#define P2P_INCLUDE_CONSTANTS_H_

#include <string>

// Universal control constants utilized by both registration server and P2P Clients

inline const std::string kDone = "DONE"; // Message indicates all data for this section of program flow is done

inline const int kTTLDec = 7; // TTLs shall be decremented every this many seconds

inline const int MSG_LEN = 1024; // Size that all message buffers are initialized to

inline const int kTimeoutAttempts = 5; // This is the number of retries before a host is declared down.

inline const int kKeepAliveTimeout = 120; // Seconds between keepalive pings

inline const int kEmptyBufferSleep = 10000; // Quantity of microseconds the system should sleep for when it expects more msgs.


// Registration server constants and methods

inline const int kControlPort = 65432; // The port listening on the RS

inline const std::string kCliRegister = "CREG"; // Client requests to register to the RS

inline const std::string kCliRegAck = "CACK"; // Response acking this client registration and providing port, cookie

inline const std::string kPeerListItem = "PEER"; // Response from RS signaling message contains data for one (active) Peer

inline const std::string kGetPeerList = "PLST"; // Registered client requesting the updated peer list

inline const std::string kKeepAlive = "ALIV"; // Message indicating this client is still alive

inline const std::string kLeave = "LEAV"; // Client indicating they are leaving the system


// P2P Client constants and control methods

inline const std::string kGetIndex = "LIST"; // Request from a peer asking for this peer's copy of the Distributed index

inline const std::string kIndexItem = "LSTI"; // Response to a peer containing a member of the distributed index on this host

inline const std::string kGetFile = "GETF"; // Request from a peer for a file located on this host

inline const std::string kNackFile = "NCKF"; // Response from a peer that this file isn't available from this peer right now (Locked by another transmission)

inline const std::string kSendingFile = "SEND"; // Response from a peer confirming that the peer will commence transmitting the file

inline const std::string kFileLine = "DATA"; // Response from a peer indicating this message will contain a line of the file being transmitted



#endif /* INCLUDE_CONSTANTS_H_ */
