/*
 * SocketInputStream.h
 *
 *  Created on: Dec 20, 2013
 *      Author: kazu
 */

#ifndef SOCKETINPUTSTREAM_H_
#define SOCKETINPUTSTREAM_H_

#include "../streams/InputStream.h"

#include "../streams/StreamException.h"
#include "Socket.h"

namespace K {

	class SocketInputStream : public InputStream {

	public:

		/** ctor */
		SocketInputStream(Socket& sck) : sck(sck) {
			;
		}

		/** dtor */
		~SocketInputStream() {
			;
		}


		int read() override {
			uint8_t data;
			int ret = read(&data, 1);
			if (ret == -1) {return ret;}
			return data;
		}

		int read(uint8_t* data, unsigned int len) override {
			return sck.read(data, len);
		}

		void close() override {
			sck.close();
		}

		void skip(uint64_t n) override {
			(void) n;
			throw StreamException("socket.skip() not yet implemented!");
		}

	private:

		friend class Socket;

		/** the socket to read from */
		Socket& sck;

	};

}

#endif /* SOCKETINPUTSTREAM_H_ */
