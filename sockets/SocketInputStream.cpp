/*
 * SocketInputStream.cpp
 *
 *  Created on: Dec 20, 2013
 *      Author: kazu
 */


#include "SocketInputStream.h"
#include "../streams/StreamException.h"
#include "Socket.h"

namespace K {

SocketInputStream::SocketInputStream(Socket& sck) : sck(sck) {
	;
}

SocketInputStream::~SocketInputStream() {
	;
}

int SocketInputStream::read() {
	uint8_t data;
	int ret = read(&data, 1);
	if (ret == -1) {return ret;}
	return data;
}


int SocketInputStream::read(uint8_t* data, unsigned int len) {
	return sck.read(data, len);
}

void SocketInputStream::close() {
	sck.close();
}

void SocketInputStream::skip(uint64_t n) {
	throw StreamException("socket.skip() not yet implemented!");
}

}

