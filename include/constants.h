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

// Universal control constants utilized by both registration server and P2P Clients

inline const char *kDone = "DONE"; // Message indicates all data for this section of program flow is done

inline const int kTTLDec = 30; // TTLs shall be decremented every this many seconds

inline const int MSG_LEN = 1024; // Size that all message buffers are initialized to


// Registration server constants and methods

inline const int kControlPort = 65432; // The port listening on the RS

inline const char *kCliRegister = "CREG"; // Client requests to register to the RS

inline const char *kCliRegAck = "CACK"; // Response acking this client registration and providing port, cookie

inline const char *kPeerListItem = "PEER"; // Response from RS signaling message contains data for one (active) Peer

inline const char *kKeepAlive = "ALIV"; // Message indicating this client is still alive

inline const char *kLeave = "LEAV"; // Client indicating they are leaving the system


// P2P Client constants and control methods

inline const char *kGetIndex = "LIST"; // Request from a peer asking for this peer's copy of the Distributed index

inline const char *kIndexItem = "LSTI"; // Response to a peer containing a member of the distributed index on this host

inline const char *kGetFile = "GETF"; // Request from a peer for a file located on this host

inline const char *kNackFile = "NCKF"; // Response from a peer that this file isn't available from this peer right now (Locked by another transmission)

inline const char *kSendingFile = "SEND"; // Response from a peer confirming that the peer will commence transmitting the file

inline const char *kFileLine = "DATA"; // Response from a peer indicating this message will contain a line of the file being transmitted



#endif /* INCLUDE_CONSTANTS_H_ */
