#ifndef K_NET_TCPSTREAM_H
#define K_NET_TCPSTREAM_H

#include "../Payload.h"
#include "../ipv4/PacketIPv4.h"

#include "PacketTCP.h"
#include "TCPStreamBuffer.h"

namespace K {

	static constexpr const char* K_DBG_TCP_STREAM = "TCP_STREAM";

	/**
	 * reassemble TCP-packets to a continuous data stream.
	 * this class works for one stream only.
	 * all added packets must belong to the same stream (same src and dst)!
	 */
	class TCPStream {

	private:

		/** the buffer used for reassembly */
		TCPReassemblyBuffer buffer;

		/** last seq nr */
		uint32_t seqNr;

		/** total number of added packets */
		uint32_t numPacketsAdded;

		/** total number of retransmitted packets */
		uint32_t numPacketsRetransmitted;

		/** total received payload (in bytes) */
		uint32_t payloadSize;

	public:

		/** ctor */
		TCPStream() : seqNr(0), numPacketsAdded(0), numPacketsRetransmitted(0), payloadSize(0) {
			;
		}

		/** dtor */
		~TCPStream() {
			;
		}

		/** get the number of added packets */
		int getNumPacketsAdded() const {return numPacketsAdded;}

		/** get the total number of received bytes */
		int getPayloadSize() const {return payloadSize;}

		/** get the number of retransmitted packets */
		int getNumPacketsRetransmitted() const {return numPacketsRetransmitted;}

		/** append this tcp packet. will return usable payload as soon as something valid is available */
		Payload add(const PacketTCP& tcp) {
			const Payload p = get(tcp);
			payloadSize += p.length;
			return p;
		}

	private:

		/** see add() */
		Payload get(const PacketTCP& tcp) {

			++numPacketsAdded;

			debug(K_DBG_TCP_STREAM, "adding TCP frame. seqNr: " << tcp.getSeqNumber());

			// get the TCP's payload
			Payload p = tcp.getPayload();

			// allow start-within (without handshake)
			if (this->seqNr == 0) {
				debug(K_DBG_TCP_STREAM, "starting TCP stream");
				seqNr = tcp.getSeqNumber() + p.length;
				return p;
			}

			// retransmitted frame? -> drop
			if (tcp.getSeqNumber() < seqNr) {
				debug(K_DBG_TCP_STREAM, "dropping retransmitted frame");
				++numPacketsRetransmitted;
				return Payload::empty();
			}

			// works without reassembly?
			if (tcp.getSeqNumber() == seqNr && buffer.isEmpty()) {
				debug(K_DBG_TCP_STREAM, "no reassembly needed");
				seqNr = tcp.getSeqNumber() + p.length;
				return p;
			}

			// frame(s) missing before this frame. update the reassembly buffer
			debug(K_DBG_TCP_STREAM, "missing frames before this one. appending to buffer.");
			Payload pRes = buffer.append(seqNr, tcp.getSeqNumber(), p);
			seqNr += pRes.length;
			return pRes;

		}



	};

}

#endif // K_NET_TCPSTREAM_H
