/*
 * DataOutputStream.h
 *
 *  Created on: Dec 19, 2013
 *      Author: kazu
 */

#ifndef DATAOUTPUTSTREAM_H_
#define DATAOUTPUTSTREAM_H_

#include "Endian.h"
#include "OutputStream.h"


namespace K {


/**
 * write several data-types to an underlying output stream
 */
class DataOutputStream : public OutputStream {

public:

	/** ctor */
	DataOutputStream(OutputStream& os) : os(os) {
		;
	}

	/** dtor */
	~DataOutputStream() {
		;
	}

	void write(uint8_t data) override {
		os.write(data);
	}

	void write(uint8_t* data, unsigned int len) override {
		os.write(data, len);
	}

	void flush() override {
		os.flush();
	}

	void close() override {
		os.close();
	}

	/** write one byte */
	void writeByte(uint8_t data) {
		write(data);
	}

	/** write one word */
	void writeShort(uint16_t data) {
		data = Endian::toLittleEndian(data);
		write((uint8_t*)&data, 2);
	}

	/** write one int */
	void writeInt(uint32_t data) {
		data = Endian::toLittleEndian(data);
		write((uint8_t*)&data, 4);
	}

	/** write one long */
	void writeLong(uint64_t data) {
		data = Endian::toLittleEndian(data);
		write((uint8_t*)&data, 8);
	}

	/** write one float */
	void writeFloat(float data) {
		data = Endian::toLittleEndian(data);
		write((uint8_t*)&data, 4);
	}

	/** write one double */
	void writeDouble(double data) {
		data = Endian::toLittleEndian(data);
		write((uint8_t*)&data, 8);
	}


private:

	/** the underlying output stream */
	OutputStream& os;

};

}



#endif /* DATAOUTPUTSTREAM_H_ */
