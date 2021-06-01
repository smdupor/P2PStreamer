/*
 * Util.h
 *
 *  Created on: May 31, 2021
 *      Author: smdupor
 */

#ifndef INCLUDE_UTIL_H_
#define INCLUDE_UTIL_H_

#include <iostream>
#include <vector>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


class Util {
public:
	static std::vector<std::string> split(const std::string &input, char delim);
};

#endif /* INCLUDE_UTIL_H_ */
