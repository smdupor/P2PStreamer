/*
 * NetworkCommunicator.cpp
 *
 *  Created on: May 31, 2021
 *      Author: smdupor
 */

#include "NetworkCommunicator.h"
#include "P2PClient.h"

NetworkCommunicator::~NetworkCommunicator() {
   //Empty Destructor -- Override in Subclasses

}

int NetworkCommunicator::listener(int listen_port) {
   int sockfd; // socket descriptor
   struct sockaddr_in serv_addr; //socket addresses

   // Create the socket
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) {
      error("ERROR opening socket");
      return -1;
   }

   // Initialize address and port values
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(listen_port);

   // Bind the socket
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      error("Error on socket bind");
      return -1;
   }

   // Listen for new connections
   verbose("LISTENING FOR CONNECTIONS on port: " + std::to_string(listen_port));
   listen(sockfd, 12);
   return sockfd;
}

int NetworkCommunicator::outgoing_connection(std::string hostname, int port) {
   struct sockaddr_in serv_addr;
   struct hostent *server;

   // int port = kControlPort;
   int sockfd;

   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) {
      error("Unable to open connection to: " + hostname + ". Sleeping.");
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      return -1;
   }

   server = gethostbyname(hostname.c_str());

   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);

   serv_addr.sin_port = htons(port);
   if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      verbose("ERROR connecting to remote socket");
      return -1;
   }
   return sockfd;
}

int NetworkCommunicator::get_port() {
   return this->port;
}

void NetworkCommunicator::ttl_decrementer() {
   //Override in subclasses
}

void NetworkCommunicator::transmit(int sockfd, std::string &out_message) {
   int n;
   const char *out_buffer;

   out_buffer = out_message.c_str();
   // print_sent(out_message);
   n = write(sockfd, (const char *) out_buffer, strlen((const char *) out_buffer));

   if (n < 0)
      verbose("Error on write to buffer");
}

std::string NetworkCommunicator::receive(int sockfd) {
   int n = 0, timeout_counter = 0;
   char *in_buffer[MSG_LEN * 2];
   bzero(in_buffer, MSG_LEN * 2);

   std::string in_message = std::string("");

   // As long as data is coming in, keep reading.
   while (true) {
      bzero(in_buffer, MSG_LEN * 2);
      n = read(sockfd, in_buffer, MSG_LEN * 2);

      if (n < 0 || timeout_counter > kTimeoutRetry) {
         if(system_on)
            error("Error in reading socket");
         return kDone + " \n";
      } else if (std::strlen((char *) in_buffer) == 0) {
         std::this_thread::sleep_for(std::chrono::microseconds(kEmptyBufferSleep));
         timeout_counter += 1;
      } else {
         in_message += std::string((char *) in_buffer);
         timeout_counter = 1;
      }

      // If we determine that we've got the entire message
      if (!in_message.empty() && in_message.substr(in_message.length() - 2) == "\n\n") {
         if(system_on)
            print_recv(in_message);
         in_message = in_message.substr(0, in_message.length() - 1); // Strip the extra newline
         return in_message;
      }
   }
}

std::string NetworkCommunicator::receive_no_delim(int sockfd) {
   int n;
   char *in_buffer[MSG_LEN * 2];
   bzero(in_buffer, MSG_LEN * 2);

   std::string in_message = "";

   n = read(sockfd, in_buffer, MSG_LEN * 2);
   if (n < 0)
      verbose("Error in reading socket, Called by:" + debug);

   in_message = std::string((char *) in_buffer);
   print_recv(in_message);
   return in_message;
}

// Split up a string into a vector of substrings based on delimiter character
std::vector<std::string> NetworkCommunicator::split(const std::string &input, char delim) {
   std::vector<std::string> split_string; // Vector of string tokens to be returned
   size_t start = 0, end = 0; // Moving indices

   // Iterate through input string
   while ((end = input.find(delim, start)) != std::string::npos) {
      split_string.push_back(input.substr(start, end - start));
      start = end + 1;
   }

   // Grab the last token indicated by no more delimiters found in string
   if (!input.substr(start).empty()) {
      split_string.push_back(input.substr(start));
   }
   return split_string;
}

void NetworkCommunicator::print_sent(std::string input) { // Print sent data in dark yellow
   std::cout << "\033[33m" << input << "\033[0m";
   std::cout.flush();
}

void NetworkCommunicator::print_recv(std::string input) { // Print receved data in green
   std::cout << "\033[32m" << input << "\033[0m";
   std::cout.flush();

}

// Prints debug output to the terminal when "verbose" mode is in use.
void NetworkCommunicator::verbose(std::string input) {
   if (debug) {
      std::cout << "\033[35m" << input << "\033[0m" << std::endl;
   }
}

void NetworkCommunicator::error(std::string input) { // bright red
   std::cout << "\033[91m" << input << "\033[0m";
}

void NetworkCommunicator::warning(std::string input) { // bright yellow
   std::cout << "\033[93m" << input << "\033[0m";
}

//cyan
void NetworkCommunicator::info(std::string input) {
   std::cout << "\033[36m" << input << "\033[0m";
}