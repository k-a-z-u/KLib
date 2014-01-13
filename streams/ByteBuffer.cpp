/*
 * ByteBuffer.cpp
 *
 *  Created on: Dec 15, 2013
 *      Author: kazu
 */

#include "ByteBuffer.h"
#include <cstdlib>

namespace K {

ByteBuffer::ByteBuffer() : data(0) {
	resize(4096);
}

ByteBuffer::~ByteBuffer() {
	if(data) {free(data);}
}

unsigned int ByteBuffer::freeBytes() {
	return size - used;
}

void ByteBuffer::ensureSpaceFor(unsigned int size) {
	unsigned int free = freeBytes();
	if (free > size) {return;}
	unsigned int newSize = (this->size + size) * 2;
	resize(newSize);
}

void ByteBuffer::write(uint8_t* data, int len) {
	ensureSpaceFor(len);
}


void ByteBuffer::resize(unsigned int size) {
	data = (uint8_t*) realloc(data, size);
	this->size = size;
}

}
