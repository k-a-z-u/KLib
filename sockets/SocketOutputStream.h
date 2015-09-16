/*
 * SocketOutputStream.h
 *
 *  Created on: Dec 20, 2013
 *      Author: kazu
 */

#ifndef SOCKETOUTPUTSTREAM_H_
#define SOCKETOUTPUTSTREAM_H_


#include "../streams/OutputStream.h"
#include "Socket.h"

namespace K {

	class Socket;

	class SocketOutputStream : public OutputStream {

	public:

		/** ctor */
		SocketOutputStream(Socket* sck) : sck(sck) {
			;
		}

		/** dtor */
		~SocketOutputStream() {
			;
		}

		void write(uint8_t data) override {
			write(&data, 1);
		}

		void write(const uint8_t* data, const size_t len) override {
			sck->write(data, len);
		}

		void flush() override {
			;
		}

		void close() override {
			sck->close();
		}

	private:

		/** the socket to write to */
		Socket* sck;

	};

}



#endif /* SOCKETOUTPUTSTREAM_H_ */
