/*
 * NetworkCommunicator.cpp
 *
 *  Created on: May 31, 2021
 *      Author: smdupor
 */

#include "NetworkCommunicator.h"

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

NetworkCommunicator::~NetworkCommunicator() {
	//Empty Destructor

}
