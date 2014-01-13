/*
 * ServerSocket.cpp
 *
 *  Created on: Dec 15, 2013
 *      Author: kazu
 */

#include "ServerSocket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "SocketException.h"

namespace K {

ServerSocket::ServerSocket() : handle(0) {

}

ServerSocket::~ServerSocket() {
	close();
}

void ServerSocket::bind(const SckPort port) {

	// cleanup
	if (handle) {::close(handle); handle = 0;}

	// create socket
	handle = socket(AF_INET, SOCK_STREAM, 0);
	if (!handle) {throw SocketException("error creating server socket");}

	// bind to the given port
	struct sockaddr_in srvAddr;
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_addr.s_addr = INADDR_ANY;
	srvAddr.sin_port = htons(port);
	int ret = ::bind(handle, (struct sockaddr*) &srvAddr, sizeof(srvAddr));
	if (ret < 0) {throw SocketException("error while binding server socket");}

	// start listening
	::listen(handle, 5);

}

Socket* ServerSocket::accept() {

	struct sockaddr_in cliAddr;
	socklen_t addrLen = sizeof(cliAddr);

	int newHandle = ::accept(handle, (struct sockaddr*) &cliAddr, &addrLen);
	if (newHandle < 0) {throw SocketException("error while accepting client connection");}

	return new Socket(newHandle);

}

void ServerSocket::close() {
	if (handle) {::close(handle); handle = 0;}
}

}

