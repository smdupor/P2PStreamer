/*
 * FileReceiver.h
 *
 *  Created on: May 20, 2021
 *      Author: smdupor
 */

#ifndef FILERECEIVER_H_
#define FILERECEIVER_H_

class FileReceiver {
public:
	FileReceiver();
	virtual ~FileReceiver();
	int receive(char *fileName, int sockfd);
	void error(const char *msg);

};

#endif /* FILERECEIVER_H_ */
