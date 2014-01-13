/*
 * SocketInputStream.h
 *
 *  Created on: Dec 20, 2013
 *      Author: kazu
 */

#ifndef SOCKETINPUTSTREAM_H_
#define SOCKETINPUTSTREAM_H_

#include "../streams/InputStream.h"


namespace K {

class Socket;

class SocketInputStream : public InputStream {

public:

	/** ctor */
	SocketInputStream(Socket& sck);

	/** dtor */
	~SocketInputStream();


	int read() override;

	int read(uint8_t* data, unsigned int len) override;

	void close() override;

private:

	friend class Socket;

	/** the socket to read from */
	Socket& sck;

};

}

#endif /* SOCKETINPUTSTREAM_H_ */
