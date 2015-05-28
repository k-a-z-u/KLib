#ifndef K_NET_RAW_SNIFFER_SNIFFERLISTENER_H
#define K_NET_RAW_SNIFFER_SNIFFERLISTENER_H

#include <cstdint>

namespace K {

	/**
	 * interface to listen for captured packets within the network sniffer
	 */
	class SnifferListener {

	public:

		/** dtor */
		virtual ~SnifferListener() {;}

		/** received a new data packet */
		virtual void onPacket(const uint8_t* data, const uint32_t len, const struct timeval ts) = 0;

	};

}

#endif // SNIFFERLISTENER_H
