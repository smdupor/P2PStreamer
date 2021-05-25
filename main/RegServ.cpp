//============================================================================
// Name        : RS.cpp
// Author      : Stevan Dupor
// Version     : 0.1
// Description : Main file for Registration Server for P2P-DI
//============================================================================

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <ctime>
#include <algorithm>

#include "BasicClient.h"
#include "BasicServer.h"
#include "PeerNode.h"

int main(void) {

	std::list <PeerNode> peers;
	peers.push_back(PeerNode("asdfasdf", 1, 2));
	peers.push_back(PeerNode("jk;ljkli", 3, 4));
	std::for_each(peers.begin(), peers.end(), [](PeerNode node) {
		std::cout << node.toS() << '\n';
	});

	return EXIT_SUCCESS;
}

