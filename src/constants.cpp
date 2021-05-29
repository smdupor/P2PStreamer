/*
 * constants.cpp
 *
 *  Created on: May 28, 2021
 *      Author: smdupor
 */
/*
#include "constants.h"


// Universal control constants utilized by both registration server and P2P Clients

*done = "DONE"; // Message indicates all data for this section of program flow is done

ttlDecrement = 30; // TTLs shall be decremented every this many seconds



// Registration server constants and methods

controlPort = 65432; // The port listening on the RS

*cliRegister = "CREG"; // Client requests to register to the RS

*cliRegAck = "CACK"; // Response acking this client registration and providing port, cookie

*peerListItem = "PEER"; // Response from RS signaling message contains data for one (active) Peer

*keepAlive = "ALIV"; // Message indicating this client is still alive

*leave = "LEAV"; // Client indicating they are leaving the system


// P2P Client constants and control methods

*getIndex = "LIST"; // Request from a peer asking for this peer's copy of the Distributed index

*indexItem = "LSTI"; // Response to a peer containing a member of the distributed index on this host

*getFile = "GETF"; // Request from a peer for a file located on this host

*nackFile = "NCKF"; // Response from a peer that this file isn't available from this peer right now (Locked by another transmission)

*sendingFile = "SEND"; // Response from a peer confirming that the peer will commence transmitting the file

*fileLine = "DATA"; // Response from a peer indicating this message will contain a line of the file being transmitted



*/
