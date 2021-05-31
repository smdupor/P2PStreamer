/*
 * BasicServer.h
 *
 *  Created on: May 19, 2021
 *      Author: smdupor
 */

#ifndef BASICSERVER_H_
#define BASICSERVER_H_

	struct sockinfo {
		int socket;
		char * cli_addr;
	};

class BasicServer {
private:
	int PORTNUM;
	int sockfd;
	int newsockfd;

public:

	BasicServer(int);
	virtual ~BasicServer();
	int getPort();
	int getAddr();
	sockinfo start();
	int listener();
	sockinfo accepter(int);
	bool registration(sockinfo);

};

#endif /* BASICSERVER_H_ */
