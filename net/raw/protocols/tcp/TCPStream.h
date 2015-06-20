#ifndef K_NET_TCPSTREAM_H
#define K_NET_TCPSTREAM_H

#include "../Payload.h"
#include "../ipv4/PacketIPv4.h"

#include "PacketTCP.h"
#include "TCPStreamBuffer.h"
#include "TCPStreamQueue.h"

namespace K {

	static constexpr const char* K_DBG_TCP_STREAM = "TCP_STREAM";

	/**
	 * reassemble TCP-packets to a continuous data stream.
	 * this class works for one stream only.
	 * all added packets must belong to the same stream (same src and dst)!
	 */
	class TCPStream {

	private:

		///** the buffer used for reassembly */
		//TCPReassemblyBuffer buffer;

		/** the queue used for reassembly */
		TCPStreamQueue queue;


//		/** is this stream new? (no packets added) */
//		bool isNew;

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

		/** user defined something that may be attached to the stream (e.g. a handler) */
		void* userPtr;


	public:

		/** ctor */
		TCPStream(const PacketTCP& tcp) :
			numPacketsAdded(0), numPacketsRetransmitted(0), numPacketsOutOfOrder(0), payloadSize(0) {

			nextSeqNr = tcp.getSeqNumber() + getSeqNrInc(tcp);
			ports.src = tcp.getSrcPort();
			ports.dst = tcp.getDstPort();

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

		/** attach a user-defined something to this stream. e.g. a handler or something that belongs to it */
		void setUserPointer(void* userPtr) {this->userPtr = userPtr;}

		/** get the user-defined something attached to this stream */
		void* getUserPointer() const {return userPtr;}

		/** append this tcp packet. will return usable payload as soon as something valid is available */
		std::vector<PacketTCP> add(const PacketTCP& tcp) {
			std::vector<PacketTCP> packets = get(tcp);
			for (const PacketTCP& pkt : packets) {payloadSize += pkt.getPayloadLength();}
			return packets;
		}

		/** get this stream's dst port */
		uint16_t getDstPort() const {return ports.dst;}

		/** get this stream's src port */
		uint16_t getSrcPort() const {return ports.src;}


	private:

		/** see: add() */
		std::vector<PacketTCP> get(const PacketTCP& tcp) {

			++numPacketsAdded;
			std::vector<PacketTCP> packets;

			debug(K_DBG_TCP_STREAM, "adding TCP frame. seqNr: " << tcp.getSeqNumber());

			// get the TCP's payload
			//Payload p = tcp.getPayload();

			// sanity check
			if (tcp.getSrcPort() != ports.src || tcp.getDstPort() != ports.dst) {
				throw Exception("tcp ports have changed?! are you trying to add packets from another connection into this tcp-stream?");
			}

			// retransmitted frame? -> drop
			if (tcp.getSeqNumber() < nextSeqNr) {
				debug(K_DBG_TCP_STREAM, "dropping retransmitted frame");
				++numPacketsRetransmitted;
				return packets;
				//return Payload::empty();
			}

			// works without reassembly?
			if (tcp.getSeqNumber() == nextSeqNr && queue.isEmpty()) {
				debug(K_DBG_TCP_STREAM, "no reassembly needed");
				nextSeqNr += getSeqNrInc(tcp);
				packets.push_back(tcp);
				return packets;
			}

			// frame(s) missing before this frame. update the reassembly buffer
			debug(K_DBG_TCP_STREAM, "missing frames before this one. appending to buffer.");
			//Payload pRes = q.append(nextSeqNr, tcp.getSeqNumber(), p);
//			if (!pRes.isEmpty()) {
//				++numPacketsOutOfOrder;
//				nextSeqNr += pRes.length;//getSeqNrInc(tcp);
//			}

			// append the current packet into the queue
			queue.append(tcp);

			// stats
			if (!queue.hasNext(nextSeqNr)) {++numPacketsOutOfOrder;}

			// check whether the queue contains usable data
			while(queue.hasNext(nextSeqNr)) {
				const PacketTCP next = queue.next();
				packets.push_back(next);
				nextSeqNr += getSeqNrInc(next);
			}

			// done
			return packets;

		}



	private:

		uint32_t getSeqNrInc(const PacketTCP& tcp) const {
			return tcp.getPayload().length + (tcp.isSyn() ? 1 : 0);
		}


	};

}

#endif // K_NET_TCPSTREAM_H
