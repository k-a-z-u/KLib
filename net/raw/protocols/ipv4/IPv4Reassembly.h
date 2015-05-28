#ifndef IPV4REASSEMBLY_H
#define IPV4REASSEMBLY_H

#include "PacketIPv4.h"
#include "../Payload.h"
#include <string.h>

namespace K {

	/**
	 * reassemble TCP-packets to a continuous data stream.
	 * this class works for one stream only.
	 * all added packets must belong to the same stream (same src and dst)
	 */
	class IPv4Reassembly {

	private:

		/** internal data buffer for the biggest possible size of a reassembly (64KiB) */
		uint8_t* buffer;

		/** the number of bytes we got from a fragment */
		uint32_t curGot;

		/** the fragment's total size */
		uint32_t curSize;

	public:

		/** ctor */
		IPv4Reassembly() : curGot(0), curSize(0) {
			buffer = new uint8_t[64*1024];
		}

		/** dtor */
		~IPv4Reassembly() {
			delete buffer;
			buffer = nullptr;
		}

		/** append a new to-be-reassembled packet. returns usable payload as soon as something is available */
		Payload add(PacketIPv4* pkt) {

			// is the packet just a normal (unfragmented) IPv4 packet?
			if (pkt->moreFragements() == false && pkt->getOffset() == 0) {
				return pkt->getPayload();
			}

			// reassemble
			const Payload p = pkt->getPayload();
			memcpy(buffer + pkt->getOffset(), p.data, p.length);
			curGot += p.length;

			// do we know the total length?
			if (pkt->moreFragements() == false) {
				curSize = pkt->getOffset() + pkt->getPayloadLength();
			}

			// have we got all parts of the fragment?
			if (curSize > 0 && curGot == curSize) {
				Payload pRes(buffer, curSize);
				curSize = 0;
				curGot = 0;
				return pRes;
			}

			// nothing available yet
			return Payload::empty();

		}

	};

}

#endif // IPV4REASSEMBLY_H
