/**
 * NetworkCommunicator.cpp
 *
 * The NetworkCommunicator superclass encapsulates all functionality that is shared by both major components of the system,
 * the Registration Server and the P2P Client. This includes establishing listening sockets, establishing outgoing
 * connections, transmitting strings of data on the TCP stream, and receiving strings of data back. Any universally-shared
 * instance variables (such as port, a list of peers, verbosity flags) are also encapsulated in this superclass.
 *
 * Also, all of the messaging constants, and system-wide numeric constants (like timeout settings) are encapsulated here.
 *
 * Finally, universally shared utility methods, such as a string splitter and multiple pretty-print methods, are
 * encapsulated here.
 *
 *  Created on: May 31, 2021
 *      Author: smdupor
 */

#include "NetworkCommunicator.h"
#include "P2PClient.h"
/**
 * Destructor -- override in subclasses.
 */
NetworkCommunicator::~NetworkCommunicator() {
   //Empty Destructor -- Override in Subclasses

}

/**
 * Establish a listening TCP socket on this host.
 *
 * @param listen_port Port to listen on
 * @return a socket file descriptor for the listening socket
 */
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

/**
 * Establish a call-out connection from me to 'hostname' on 'port'
 *
 * @param hostname of the remote host
 * @param port of the remote host
 * @return a socket file descriptor for the established connection.
 */
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

/**
 * Getter method for this host's listening port
 *
 * @return listening port
 */
int NetworkCommunicator::get_port() {
   return this->port;
}

/**
 * Stub method to handle all the ttl timers. Override in subclasses.
 */
void NetworkCommunicator::ttl_decrementer() {
   //Override in subclasses
}

/**
 * Handle transmitting a string of characters to a remote host.
 *
 * @param sockfd established socket descriptor of remote host
 * @param out_message string to be sent
 */
void NetworkCommunicator::transmit(int sockfd, std::string &out_message) {
   int n;
   const char *out_buffer;

   out_buffer = out_message.c_str();
   // print_sent(out_message);
   n = write(sockfd, (const char *) out_buffer, strlen((const char *) out_buffer));

   if (n < 0)
      verbose("Error on write to buffer");
}

/**
 * Receive a "normal" or "control" message, e.g. a message that contains NO file transfer payload (with unexpected
 * character sequences like multiple-newlines, etc. Inspects contents of the data it is reading, and continues to read
 * data until a "message-end" sequence of \n\n is detected.
 *
 * @param sockfd socket descriptor to read data from
 * @return string of characters read from socket stream -- a COMPLETE series of messages.
 */
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

/**
 * (RAW) Receive generalized data from a socket stream (File payload data) that will not be formatted in standard messaging
 * format. Unlike receive(), this method continues to read as long as it is called, and ignores the contents of what it
 * is reading. May return incomplete/partial messages or lines of file payload.
 *
 * @param sockfd
 * @return
 */
std::string NetworkCommunicator::receive_no_delim(int sockfd) {
   int n;
   char *in_buffer[MSG_LEN * 2];
   bzero(in_buffer, MSG_LEN * 2);

   std::string in_message = "";

   n = read(sockfd, in_buffer, MSG_LEN * 2);
   if (n < 0)
      verbose("Error in reading socket, Called by:" + debug);

   in_message = std::string((char *) in_buffer);
   // Suppress printing of files in transmission to the console
   // print_recv(in_message);
   return in_message;
}

/**
 * Utility method that splits an input string into a vector of tokens based on the delimiter character, DROPPING the
 * delimiter character.
 *
 * @param input the string to be split up
 * @param delim the character to split by (eg, space or newline)
 * @return a std::vector of strings that have been split into tokens with the delimiter removed.
 */
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

/** (ent data)
 * Print data (that has been sent by this host) in dark yellow.
 * @param input the string to print
 */
void NetworkCommunicator::print_sent(std::string input) { // Print sent data in dark yellow
   std::cout << "\033[33m" << input << "\033[0m";
   std::cout.flush();
}

/**(received data)
 * Print data (that has been received by this host) in green.
 * @param input the string to print
 */
void NetworkCommunicator::print_recv(std::string input) { // Print receved data in green
   std::cout << "\033[32m" << input << "\033[0m";
   std::cout.flush();

}

/** (verbose mode support)
 * When system is running in "verbose" mode, print the string to the console in purple.
 * @param input the string to print
 */
void NetworkCommunicator::verbose(std::string input) {
   if (debug) {
      std::cout << "\033[35m" << input << "\033[0m" << std::endl;
   }
}

/** (system errors)
 * Print the string to the console in Bright Red.
 * @param input the string to print
 */
void NetworkCommunicator::error(std::string input) { // bright red
   std::cout << "\033[91m" << input << "\033[0m";
}

/** (system warnings)
 * Print the string to the console in Bright Yellow
 * @param input the string to print
 */
void NetworkCommunicator::warning(std::string input) { // bright yellow
   std::cout << "\033[93m" << input << "\033[0m";
}

/** (information messages)
 * Print the string to the console in Cyan
 * @param input the string to print
 */
void NetworkCommunicator::info(std::string input) {
   std::cout << "\033[36m" << input << "\033[0m";
}