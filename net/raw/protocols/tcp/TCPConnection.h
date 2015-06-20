#ifndef K_NET_TCPCONNECTION_H
#define K_NET_TCPCONNECTION_H

#include "TCPStream.h"

namespace K {

	struct TCPConnection {

		/** user defined something that may be attached to the stream (e.g. a handler) */
		void* userPtr;

		/** received packets */
		TCPStream* in;

		/** transmitted packets */
		TCPStream* out;


		/** ctor */
		TCPConnection() : userPtr(nullptr), in(nullptr), out(nullptr) {;}

		/** ctor */
		TCPConnection(TCPStream* in, TCPStream* out) : userPtr(nullptr), in(in), out(out) {;}


		/** get the TCPStream beloging to this dst-port */
		TCPStream* getStreamForDST(const uint16_t dstPort) {
			if (in && in->getDstPort() == dstPort) {return in;}
			if (out && out->getDstPort() == dstPort) {return out;}
			return nullptr;
		}

		/** attach a user-defined something to this stream. e.g. a handler or something that belongs to it */
		void setUserPointer(void* userPtr) {this->userPtr = userPtr;}

		/** get the user-defined something attached to this stream */
		void* getUserPointer() const {return userPtr;}


	};

}

#endif // K_NET_TCPCONNECTION_H
