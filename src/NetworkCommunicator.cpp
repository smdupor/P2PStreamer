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


// Prints debug output to the terminal when "verbose" mode is in use.
void NetworkCommunicator::verbose(std::string input) {
	if (debug){
      std::cout << "\033[35m" << input << "\033[0m" << std::endl;
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
   //print_sent(out_message);
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
  // print_recv(in_message);
   return in_message;
}

std::string NetworkCommunicator::receive(int sockfd, std::string debug) {
   int n;
   char *in_buffer[MSG_LEN];
   bzero(in_buffer, MSG_LEN);

   std::string in_message;

   n = read(sockfd, in_buffer, MSG_LEN);
   if(n<0)
      verbose("Error in reading socket, Called by:" + debug);

   // Copy buffer out to a std::string that we can work with more easily
   in_message = std::string((char *) in_buffer);
   // print_recv(in_message);
   return in_message;
}
/*
char *NetworkCommunicator::receive_cstr(int sockfd) {
   int n;
   char *in_buffer;
   bzero(in_buffer, MSG_LEN);

   n = read(sockfd, in_buffer, MSG_LEN);
   if(n<0)
      verbose("Error in reading socket");

   return in_buffer;
}*/


NetworkCommunicator::~NetworkCommunicator() {
	//Empty Destructor

}

/*************************Printers. For convenience, here is the ansi code chart:************
 * Name            FG  BG
Black           30  40
Red             31  41
Green           32  42
Yellow          33  43
Blue            34  44
Magenta         35  45
Cyan            36  46
White           37  47
Bright Black    90  100
Bright Red      91  101
Bright Green    92  102
Bright Yellow   93  103
Bright Blue     94  104
Bright Magenta  95  105
Bright Cyan     96  106
Bright White    97  107
 *
 *
 *
 */



void NetworkCommunicator::print_sent(std::string input){ // Print sent data in green
   std::cout << "\033[32m" << input << "\033[0m";
}

void NetworkCommunicator::print_recv(std::string input){ // Print sent data in green
   std::cout << "\033[31m" << input << "\033[0m";
}

void NetworkCommunicator::error(std::string input){
   std::cout << "\033[91m" << input << "\033[0m";
}