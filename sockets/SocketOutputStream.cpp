/*
 * SocketOutputStream.cpp
 *
 *  Created on: Dec 20, 2013
 *      Author: kazu
 */



#include "SocketOutputStream.h"
#include "Socket.h"

namespace K {

SocketOutputStream::SocketOutputStream(Socket& sck) : sck(sck) {
	;
}

SocketOutputStream::~SocketOutputStream() {
	;
}

void SocketOutputStream::write(uint8_t data) {
	write(&data, 1);
}

void SocketOutputStream::write(const uint8_t* data, unsigned int len) {
	sck.write(data, len);
}

void SocketOutputStream::flush() {
	;
}

void SocketOutputStream::close() {
	sck.close();
}

}
