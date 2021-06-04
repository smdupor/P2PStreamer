/*
 * NetworkCommunicator.cpp
 *
 *  Created on: May 31, 2021
 *      Author: smdupor
 */

#include "NetworkCommunicator.h"
#include "P2PClient.h"

// Split up a string into a vector of substrings based on delimiter character
std::vector<std::string> NetworkCommunicator::split(const std::string &input, char delim){
	std::vector<std::string> split_string; // Vector of string tokens to be returned
	size_t start = 0, end = 0; // Moving indices

	// Iterate through input string
	while((end = input.find(delim, start)) != std::string::npos){
		split_string.push_back(input.substr(start, end-start));
		start = end + 1;
	}

	// Grab the last token indicated by no more delimiters found in string
	split_string.push_back(input.substr(start));

	return split_string;
}


// Prints debug output to the terminal when "verbose" mode is in use
void NetworkCommunicator::verbose(std::string output) {
	if (debug){
		std::cout << output << std::endl;
	}
}

void NetworkCommunicator::ttl_decrementer() {
	//Override in subclasses
}

void NetworkCommunicator::transmit(int sockfd, std::string &out_message) {
   int n;
   const char *out_buffer;

   out_buffer = out_message.c_str();
   //std::cout << out_message;
   verbose("Sent: " + out_message);
   n = write(sockfd, (const char *) out_buffer, strlen((const char *) out_buffer));

   if (n<0)
      verbose("Error on write to buffer");

}

std::string NetworkCommunicator::receive(int sockfd) {
   int n;
   char *in_buffer[MSG_LEN];
   bzero(in_buffer, MSG_LEN);

   std::string in_message;

   n = read(sockfd, in_buffer, MSG_LEN);
   if(n<0)
      verbose("Error in reading socket");

   // Copy buffer out to a std::string that we can work with more easily
   in_message = std::string((char *) in_buffer);
   sleep(0.5);
   verbose(std::string("Recv: " + in_message));
   return in_message;
}

NetworkCommunicator::~NetworkCommunicator() {
	//Empty Destructor

}