/*
 * SocketOutputStream.h
 *
 *  Created on: Dec 20, 2013
 *      Author: kazu
 */

#ifndef SOCKETOUTPUTSTREAM_H_
#define SOCKETOUTPUTSTREAM_H_


#include "../streams/OutputStream.h"

namespace K {

class Socket;

class SocketOutputStream : public OutputStream {

public:

	/** ctor */
	SocketOutputStream(Socket& sck);

	/** dtor */
	~SocketOutputStream();

	void write(uint8_t data) override;

	void write(const uint8_t* data, unsigned int len) override;

	void flush() override;

	void close() override;

private:

	/** the socket to write to */
	Socket& sck;

};

}



#endif /* SOCKETOUTPUTSTREAM_H_ */
