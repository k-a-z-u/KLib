#ifndef K_NET_RAW_PROTOCOLS_TCP_TCPSTREAMQUEUE_H
#define K_NET_RAW_PROTOCOLS_TCP_TCPSTREAMQUEUE_H

#include "../Payload.h"
#include "../../../../Exception.h"
#include "PacketTCP.h"
#include <list>
#include <cstdint>

namespace K {

	/**
		 * the queue will enqueue and hereby re-order TCP packets that
		 * were out of sequence. as soon as the next sequence number,
		 * expected by the stream, is available within the buffer, it
		 * is returned to the caller
		 */
	class TCPStreamQueue {

	private:

		/** data of the enqueued packets */
		uint8_t* data;

		/** current position within the data buffer */
		uint32_t pos;

		/** parsed, enqueued packets */
		std::list<PacketTCP> queue;

		static constexpr int ONE_ENTRY_SIZE = 2048;
		static constexpr int NUM_ENTRIES = 64;
		static constexpr int BUF_SIZE = ONE_ENTRY_SIZE * NUM_ENTRIES;

	public:

		/** ctor */
		TCPStreamQueue() {
			reset();
			data = new uint8_t[BUF_SIZE];
			pos = 0;
		}

		/** dtor */
		~TCPStreamQueue() {
			delete(data);
			data = nullptr;
		}

		/**
			 * append the given payload (starting at packetsSeqNr)
			 * to the reassembly buffer starting at the streams next expected seq-nr
			 * and return payload as soon as something was (completely) reassembled
			 */
		void append(const PacketTCP& pkt) {

			// copy the packets data into the local buffer
			memcpy(&data[pos], pkt.getRaw(), pkt.getLength());

			// find the position within the queue where to insert the new packet (ordered by seq-nr)
			auto it = queue.begin();
			while (it != queue.end()) {
				if (it->getSeqNumber() > pkt.getSeqNumber()) {break;}
				++it;
			}

			// insert (reordered) the packet before "it"
			queue.insert(it, PacketTCP::wrap(&data[pos], pkt.getLength()));

			// circular buffer
			pos += pkt.getLength();
			if ((BUF_SIZE - pos) < ONE_ENTRY_SIZE) {pos = 0;}

		}

		/** does the buffer have read-to-use packets? */
		bool hasNext(const uint32_t nextExpectedSeqNr) const {
			if (queue.empty()) {return false;}
			return queue.front().getSeqNumber() == nextExpectedSeqNr;
		}

		/** get the next TCP-Packet from the buffer */
		PacketTCP next() {
			PacketTCP tcp = queue.front();
			queue.pop_front();
			return tcp;
		}

		/** clear the buffer */
		void reset() {
			queue.clear();
			pos = 0;
		}

		/** is the buffer currently empty? */
		bool isEmpty() const {
			return queue.empty();
		}

	};

}

#endif //K_NET_RAW_PROTOCOLS_TCP_TCPSTREAMQUEUE_H
