/*
 * DataInputStream.h
 *
 *  Created on: Dec 19, 2013
 *      Author: kazu
 */

#ifndef DATAINPUTSTREAM_H_
#define DATAINPUTSTREAM_H_

#include "Endian.h"
#include "InputStream.h"
#include "StreamException.h"

namespace K {

class DataInputStream : public InputStream {

public:

	/** ctor */
	DataInputStream(InputStream& is) : is(is) {
		;
	}

	/** dtor */
	~DataInputStream() {
		;
	}

	int read() override {
		return is.read();
	}

	int read(uint8_t* data, unsigned int len) override {
		return is.read(data, len);
	}

	uint8_t readByte() {
		int i = read();
		if (i == -1) {throw StreamException("reading error");}
		return (uint8_t) i;
	}

	uint16_t readShort() {
		uint16_t ret;
		readFully((uint8_t*)&ret, 2);
		return Endian::fromLittleEndian(ret);
	}

	uint32_t readInt() {
		uint32_t ret;
		readFully((uint8_t*)&ret, 4);
		return Endian::fromLittleEndian(ret);
	}

	uint64_t readLong() {
		uint64_t ret;
		readFully((uint8_t*)&ret, 8);
		return Endian::fromLittleEndian(ret);
	}

	float readFloat() {
		float ret;
		readFully((uint8_t*)&ret, 4);
		return Endian::fromLittleEndian(ret);
	}

	double readDouble() {
		double ret;
		readFully((uint8_t*)&ret, 8);
		return Endian::fromLittleEndian(ret);
	}

	void close() override {
		is.close();
	}

	void skip(const uint64_t n) override {
		is.skip(n);
	}

private:

	/** the underlying input stream */
	InputStream& is;

};

}


#endif /* DATAINPUTSTREAM_H_ */
