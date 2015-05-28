#ifndef K_NET_TCPCONNECTION_H
#define K_NET_TCPCONNECTION_H

#include "TCPStream.h"

namespace K {

	struct TCPConnection {

		/** received packets */
		TCPStream in;

		/** transmitted packets */
		TCPStream out;

	};

}

#endif // K_NET_TCPCONNECTION_H
