/*
 * RegistrationServer.cpp
 *
 *  Created on: May 28, 2021
 *      Author: smdupor
 */

#include "RegistrationServer.h"

// Constructor to initialize the registration server and set up logging
RegistrationServer::RegistrationServer(std::string logfile, bool verbose_debug) : NetworkCommunicator() {
	// Initialize instance variables
	log = logfile;
	latest_cookie = 1;
	lock=false;
	port = kControlPort;
	debug = verbose_debug;
	start_time = (const time_t) std::time(nullptr);
}

RegistrationServer::~RegistrationServer() {
	// TODO Auto-generated destructor stub
}

// Starts the registration server, creates a socket and listens/accepts/handles new connections
int RegistrationServer::start(){
	int sockfd; // socket descriptor
	socklen_t clilen; //client length
	struct sockaddr_in serv_addr, cli_addr; //socket addresses
	sockinfo accepted_socket; // Values passed on once a connection is accepted

	sockfd = listener(this->port);

	clilen = sizeof(cli_addr);

	// Loop continuously to accept new connections
	while(1){

		//accept the connection
		accepted_socket.socket = (int) accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

		//convert the hostname to a useable datatype
		struct sockaddr_in* ipV4Addr = (struct sockaddr_in*)&cli_addr;
		struct in_addr ipAddr = ipV4Addr->sin_addr;
		accepted_socket.cli_addr = new char[INET_ADDRSTRLEN];
		inet_ntop( AF_INET, &ipAddr, accepted_socket.cli_addr, INET_ADDRSTRLEN );

		// Handle the incoming request
		accept_reg(accepted_socket);

		if (accepted_socket.socket < 0){
			verbose("Error on accepting connection");
			return -1;
		}
		//Let client close the conn
		// close(accepted_socket.socket);
	}
	return 1;
}

int RegistrationServer::accept_reg(sockinfo sock){
	// Initialize buffers
	const char * in_buffer[MSG_LEN], *out_buffer;
	bzero(in_buffer, MSG_LEN);
   int timeout_counter = 0;

	// Initialize control variables and message strings
	int n;

	std::string in_message, out_message;

   in_message = receive(sock.socket);
   std::vector<std::string> messages = split((const std::string &) in_message, '\n');
   error("messages qty:" + std::to_string(messages.size()));

      for(std::string &message : messages) {
         verbose(std::string(message + "Length: " + std::to_string(message.length())));
		// Split the string by the delimiter so we can more easily use the message data
		std::vector<std::string> tokens = split((const std::string &) message, ' ');

		//Handle client registration
		if(tokens[0] == kCliRegister) {	// First message when client wishes to register
			if(tokens[1] == "NEW"){
				// This is a new registration, handle it by adding the client and replying with ack
				out_message = kCliRegAck + create_new_peer(sock);
			}
			else{
				// This is a returning registration, and tokens[2] will contain the cookie.
            PeerNode &pn = *std::find_if(peers.begin(), peers.end(), [&](PeerNode node) {
               return node.equals(stoi(tokens[2]));});
            pn.keepAlive();
            pn.increment_reg_count();
            out_message = kCliRegAck + pn.to_msg();
			}
         transmit(sock.socket, out_message);

         // Now, reply with all other active members of the list
         for(PeerNode &p : peers){
            if(p.active()) {
               out_message = kPeerListItem + p.to_msg();
               transmit(sock.socket, out_message);
            }
         }

         // Finally, say "DONE" and close.
         out_message = kDone + " \n\n";
         transmit(sock.socket, out_message);
		}
		else if(tokens[0] == kKeepAlive) { // This is a keepalive message
			// tokens [2] will contain the cookie

			PeerNode &p = *std::find_if(peers.begin(), peers.end(), [&](PeerNode node) {
			   return node.equals(stoi(tokens[2]));});

			p.keepAlive();
			out_message = kDone + "\n\n";
         transmit(sock.socket, out_message);
		}
		else if(tokens[0] == kLeave) { // Client is leaving the system
			// tokens[2] will contain the cookie
         PeerNode &p = *std::find_if(peers.begin(), peers.end(), [&](PeerNode node) {
            return node.equals(stoi(tokens[2]));});
			p.leave();
			error(p.toS() + " Is Leaving\n");
         int killsockfd = outgoing_connection(p.get_address(), p.get_port());
         close(killsockfd);
			out_message = kDone + "\n\n";
         transmit(sock.socket, out_message);
		}
      else if(tokens[0] == kGetPeerList) { // Registered client wants the list of peers
         for(PeerNode p : peers) { //Transmit each
            out_message = kPeerListItem + p.to_msg();
            transmit(sock.socket, out_message);
         }
         //Transmit done and exit loop.
         out_message = kDone + " \n\n";
         transmit(sock.socket, out_message);
      }
		/*else if(tokens[0] == ""){ // Empty buffer
			if(timeout_counter > 10000)
			   loop_control = false;
		   usleep(100);
		   ++timeout_counter;
		}*/
		else {
			error("We received an invalid message. Dropping connection.");
		}
	} //foreach

	// Loop has exited; we are done, close socket
	//close(sock.socket);
	// Let client close socket.
	//////////////////TODO Change return type
	return 0;
}

std::string RegistrationServer::create_new_peer(sockinfo sock) {
	//create a new peernode for the new registrant
	PeerNode p = PeerNode(std::string(sock.cli_addr), latest_cookie, kControlPort+latest_cookie);
	++latest_cookie;

	// Add to list of peers
	peers.push_back(p);

	// Return the messagized version
	return p.to_msg();
}

void RegistrationServer::ttl_decrementer() {
   int seconds = kTTLDec;

	while(1) {
	   sleep(seconds);
	   std::cout << "Sleeping for " << seconds << "in timeout func\n";
	   for(PeerNode &p : peers){
         p.decTTL(seconds);
         std::cout << p.toS()<<"\n";
	   }
	}

}
/*
void RegistrationServer::ttl_decrementer() {
   ttl_decrementer(30);
}

*/