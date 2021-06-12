/*
 * NetworkCommunicator.cpp
 *
 *  Created on: May 31, 2021
 *      Author: smdupor
 */

#include "NetworkCommunicator.h"
#include "P2PClient.h"

int NetworkCommunicator::listener(int listen_port) {
   int sockfd; // socket descriptor
   socklen_t clilen; //client length
   struct sockaddr_in serv_addr, cli_addr; //socket addresses
   sockinfo accepted_socket; // Values passed on once a connection is accepted

   // Create the socket
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd<0){
      error("ERROR opening socket");
      accepted_socket.socket = -1;
      return -1;
   }

   // Initialize address and port values
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(listen_port);

   // Bind the socket
   if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
      error("Error on socket bind");
      return -1;
   }

   // Listen for new connections
   verbose("LISTENING FOR CONNECTIONS on port: " + std::to_string(listen_port));
   listen(sockfd,12);
   clilen = sizeof(cli_addr);
   return sockfd;
}

int NetworkCommunicator::get_port() {
   return this->port;
}

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
	if(!input.substr(start).empty()) {
      split_string.push_back(input.substr(start));
   }

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

void NetworkCommunicator::transmit(int sockfd, std::string out_message) {
   int n;
   const char *out_buffer;
   //usleep(50000);
   out_buffer = out_message.c_str();
   //std::cout << out_message;
   // print_sent(out_message);
   n = write(sockfd, (const char *) out_buffer, strlen((const char *) out_buffer));

   if (n<0)
      verbose("Error on write to buffer");
}



void NetworkCommunicator::transmit(int sockfd, std::string out_message, int throttle_delay_us) {

   std::this_thread::sleep_for(std::chrono::microseconds(throttle_delay_us));
   transmit(sockfd, out_message);

}
/*
std::string NetworkCommunicator::receive(int sockfd) {
   int n=0, timeout_counter=0, bytes_imm=0, past_bytes_read=0;
   char *in_buffer[1];
   bzero(in_buffer, 1);
   bool first_newline=false;

   std::string in_message = std::string("");

	 // As long as data is coming in, keep reading.
	 while(true){
	 bzero(in_buffer, 1);
   n = read(sockfd, in_buffer, 1);

   if(n<0)
      error("Error in reading socket");
	 else if(std::strlen((char *) in_buffer)==0) {
		 std::this_thread::sleep_for(std::chrono::microseconds (kEmptyBufferSleep));
		 timeout_counter += 1;
	 }
	 else if(*in_buffer[0] == '\n' && !first_newline){
      in_message += std::string((char *) in_buffer);
      first_newline = true;
	 }
	 else if (*in_buffer[0] != '\n') {
      in_message += std::string((char *) in_buffer);
      first_newline = false;
   }
	 else if(*in_buffer[0] == '\n' && first_newline);
		 return in_message;
	 }
}
*/

std::string NetworkCommunicator::receive(int sockfd) {
   int n=0, timeout_counter=0, bytes_imm=0, past_bytes_read=0;
   char *in_buffer[MSG_LEN*2];
   bzero(in_buffer, MSG_LEN*2);


   std::string in_message = std::string("");

   // As long as data is coming in, keep reading.
   while(true){
      bzero(in_buffer, MSG_LEN*2);
      n = read(sockfd, in_buffer, MSG_LEN*2);

      if(timeout_counter>200) {
        error("Error in reading socket");
        return kDone + " \n";
      }
      else if(std::strlen((char *) in_buffer)==0) {
         std::this_thread::sleep_for(std::chrono::microseconds (kEmptyBufferSleep));
         timeout_counter += 1;
      }
      else {
         in_message += std::string((char *) in_buffer);
         timeout_counter = 1;
      }
     // std::this_thread::sleep_for(std::chrono::milliseconds (100));
      // If we determine that we've got the entire message
      if(!in_message.empty() && in_message.substr(in_message.length() - 2) == "\n\n"){
         //print_recv(in_message);
         in_message = in_message.substr(0, in_message.length()-1); // Strip the extra newline
         return in_message;
      }
   }


   // print_recv(in_message);
   // We were unsuccessful, either the packet timed out, or was malformed and we are unable to continue.
   return "";
}

std::string NetworkCommunicator::receive(int sockfd, std::string debug_loc) {
   int n = 0, timeout_counter = 0, bytes_imm = 0, past_bytes_read = 0;
   char *in_buffer[MSG_LEN * 2];
   bzero(in_buffer, MSG_LEN * 2);


   std::string in_message = std::string("");

   // As long as data is coming in, keep reading.
   while (true) {
      bzero(in_buffer, MSG_LEN * 2);
      n = read(sockfd, in_buffer, MSG_LEN * 2);

      if (timeout_counter>200) {
         error("E**" + debug_loc+ " " + in_message);
         return kDone + " \n";
      } else if (std::strlen((char *) in_buffer) == 0) {
         std::this_thread::sleep_for(std::chrono::microseconds(kEmptyBufferSleep));
         timeout_counter += 1;
      } else {
         in_message += std::string((char *) in_buffer);
         timeout_counter = 1;
      }
      // std::this_thread::sleep_for(std::chrono::milliseconds (100));
      // If we determine that we've got the entire message
      if (!in_message.empty() && in_message.substr(in_message.length() - 2) == "\n\n") {
        //  print_recv(in_message);
         in_message = in_message.substr(0, in_message.length() - 1); // Strip the extra newline
         return in_message;
      }
   }
}
int NetworkCommunicator::outgoing_connection(std::string hostname, int port) {
   struct sockaddr_in serv_addr;
   struct hostent *server;

  // int port = kControlPort;
   int sockfd;

   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) {
      verbose("ERROR opening socket");
      return -1;
   }

   server = gethostbyname(hostname.c_str());

   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

   serv_addr.sin_port = htons(port);
   if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      verbose("ERROR connecting to remote socket");
      return -1;
   }
   return sockfd;
}

/** DEBUG -- REMOVING *//*
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
}*/

std::string NetworkCommunicator::receive_no_delim(int sockfd, std::string debug) {
   int n;
   char *in_buffer[MSG_LEN*2];
   bzero(in_buffer, MSG_LEN*2);

   std::string in_message="";

   n = read(sockfd, in_buffer, MSG_LEN*2);
   if(n<0)
      verbose("Error in reading socket, Called by:" + debug);

   // Copy buffer out to a std::string that we can work with more easily
   in_message = std::string((char *) in_buffer);
   // print_recv(in_message);
   //free(in_buffer);
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
   std::cout.flush();
}

void NetworkCommunicator::print_recv(std::string input){ // Print sent data in green
   std::cout << "\033[36m" << input << "\033[0m";
   std::cout.flush();

}

void NetworkCommunicator::error(std::string input){
   std::cout << "\033[91m" << input << "\033[0m";
}
