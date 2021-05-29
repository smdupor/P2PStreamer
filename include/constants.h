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

inline const char *done = "DONE"; // Message indicates all data for this section of program flow is done

inline const int ttlDecrement = 30; // TTLs shall be decremented every this many seconds



// Registration server constants and methods

inline const int controlPort = 65432; // The port listening on the RS

inline const char *cliRegister = "CREG"; // Client requests to register to the RS

inline const char *cliRegAck = "CACK"; // Response acking this client registration and providing port, cookie

inline const char *peerListItem = "PEER"; // Response from RS signaling message contains data for one (active) Peer

inline const char *keepAlive = "ALIV"; // Message indicating this client is still alive

inline const char *leave = "LEAV"; // Client indicating they are leaving the system


// P2P Client constants and control methods

inline const char *getIndex = "LIST"; // Request from a peer asking for this peer's copy of the Distributed index

inline const char *indexItem = "LSTI"; // Response to a peer containing a member of the distributed index on this host

inline const char *getFile = "GETF"; // Request from a peer for a file located on this host

inline const char *nackFile = "NCKF"; // Response from a peer that this file isn't available from this peer right now (Locked by another transmission)

inline const char *sendingFile = "SEND"; // Response from a peer confirming that the peer will commence transmitting the file

inline const char *fileLine = "DATA"; // Response from a peer indicating this message will contain a line of the file being transmitted



#endif /* INCLUDE_CONSTANTS_H_ */
