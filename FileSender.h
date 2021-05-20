/*
 * FileSender.h
 *
 *  Created on: May 20, 2021
 *      Author: smdupor
 */

#ifndef FILESENDER_H_
#define FILESENDER_H_

class FileSender {
public:
	FileSender();
	virtual ~FileSender();
	int send(char *fileName, int sockfd);
	void error(const char *msg);
};

#endif /* FILESENDER_H_ */
