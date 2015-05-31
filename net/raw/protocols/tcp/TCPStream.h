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

		/** is this stream new? (no packets added) */
		bool isNew;

		/** the expected next seq-nr */
		uint32_t nextSeqNr;

		/** total number of added packets */
		uint32_t numPacketsAdded;

		/** total number of retransmitted packets */
		uint32_t numPacketsRetransmitted;

		/** total number of out-of-order packets */
		uint32_t numPacketsOutOfOrder;

		/** total received payload (in bytes) */
		uint32_t payloadSize;

		/** ensure we are still talking on the same ports */
		struct {
			uint16_t src;
			uint16_t dst;
		} ports;


	public:

		/** ctor */
		TCPStream() : isNew(true), nextSeqNr(0), numPacketsAdded(0), numPacketsRetransmitted(0), numPacketsOutOfOrder(0), payloadSize(0) {
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

		/** get the number of ot-of-order packets */
		int getNumPacketsOutOfOrder() const {return numPacketsOutOfOrder;}

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

			// first packet added to this stream? -> set starting seq-nr
			if (isNew) {
				debug(K_DBG_TCP_STREAM, "starting TCP stream");
				isNew = false;
				nextSeqNr = tcp.getSeqNumber() + getSeqNrInc(tcp);
				ports.src = tcp.getSrcPort();
				ports.dst = tcp.getDstPort();
				return p;
			}

			// sanity check
			if (tcp.getSrcPort() != ports.src || tcp.getDstPort() != ports.dst) {
				throw Exception("tcp ports have changed?! are you trying to add packets from another connection into this tcp-stream?");
			}

			// retransmitted frame? -> drop
			if (tcp.getSeqNumber() < nextSeqNr) {
				debug(K_DBG_TCP_STREAM, "dropping retransmitted frame");
				++numPacketsRetransmitted;
				return Payload::empty();
			}

			// works without reassembly?
			if (tcp.getSeqNumber() == nextSeqNr && buffer.isEmpty()) {
				debug(K_DBG_TCP_STREAM, "no reassembly needed");
				nextSeqNr += getSeqNrInc(tcp);
				return p;
			}

			// frame(s) missing before this frame. update the reassembly buffer
			debug(K_DBG_TCP_STREAM, "missing frames before this one. appending to buffer.");
			Payload pRes = buffer.append(nextSeqNr, tcp.getSeqNumber(), p);

			if (!pRes.isEmpty()) {
				++numPacketsOutOfOrder;
				nextSeqNr += pRes.length;//getSeqNrInc(tcp);
			}

			return pRes;

		}

	private:

		uint32_t getSeqNrInc(const PacketTCP& tcp) const {
			return tcp.getPayload().length + (tcp.isSyn() ? 1 : 0);
		}


	};

}

#endif // K_NET_TCPSTREAM_H
