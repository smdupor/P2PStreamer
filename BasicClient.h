/*
 * BasicClient.h
 *
 *  Created on: May 19, 2021
 *      Author: smdupor
 */

#ifndef BASICCLIENT_H_
#define BASICCLIENT_H_

class BasicClient {
private:
	int PORTNUM;
	int sockfd;

public:
	BasicClient(int);
	virtual ~BasicClient();
	void error(const char *msg);
	int start(const char *id);
};

#endif /* BASICCLIENT_H_ */
