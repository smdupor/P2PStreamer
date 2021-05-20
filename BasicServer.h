/*
 * BasicServer.h
 *
 *  Created on: May 19, 2021
 *      Author: smdupor
 */

#ifndef BASICSERVER_H_
#define BASICSERVER_H_

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
	int start();
};

#endif /* BASICSERVER_H_ */
