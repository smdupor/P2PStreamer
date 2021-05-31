/*
 * RegistrationServer.cpp
 *
 *  Created on: May 28, 2021
 *      Author: smdupor
 */

#include "RegistrationServer.h"

// Constructor to initialize the registration server and set up logging
RegistrationServer::RegistrationServer(std::string logfile, bool verbose_debug) {
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
	int sockfd; // socket descriptors
	socklen_t clilen; //client length
	struct sockaddr_in serv_addr, cli_addr; //socket addresses
	sockinfo accepted_socket; // Values passed on once a connection is accepted

	// Create the socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd<0){
		verbose("ERROR opening socket");
		accepted_socket.socket = -1;
		return -1;
	}

	// Initialize address and port values
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	// Bind the socket
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
		verbose("Error on socket bind");
		return -1;
	}

	// Listen for new connections
	verbose("LISTENING FOR CONNECTIONS");
	listen(sockfd,10);
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
	}
	return 1;
}

int RegistrationServer::accept_reg(sockinfo sock){
	// Initialize buffers
	char * in_buffer[MSG_LEN], out_buffer[MSG_LEN];
	bzero(in_buffer, MSG_LEN);
	bzero(out_buffer, MSG_LEN);

	// Initialize control variables and message strings
	int n;
	std::string in_message, out_message;

	// Read initial input from the established socket
	n = read(sock.socket, in_buffer, MSG_LEN);
	if(n<0) {
		verbose("Error in reading socket");
	}

	// Copy buffer out to a std::string that we can work with more easily
	in_message = std::string((const char *) in_buffer);
	if(in_message.substr(0,4) == kCliRegister) {
		RegState state = NEWREG;
	}

	return 0;
}
void RegistrationServer::ttl_decrementer(){

}

void RegistrationServer::verbose(std::string output) {
	if (debug){
		std::cout << output << std::endl;
	}
}
