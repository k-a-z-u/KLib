#ifndef K_NET_TCPSTREAMBUFFER_H
#define K_NET_TCPSTREAMBUFFER_H

#include "../Payload.h"
#include "../../../../Exception.h"

#include <vector>
#include <cstdint>


namespace K {

	static constexpr const char* K_DBG_TCP_STREAM_BUFFER = "TCP_STRBUF";

	/**
	 * represents the data-buffer for an unidirectional tcp stream.
	 * this buffer allows closing gaps due to packets received
	 * in a wrong order.
	 */
	class TCPReassemblyBuffer {

		/** the sequence number the buffer starts at (this was the last packet that was received "OK") */
		uint32_t startSeqNr;

		/** the buffers current end seq-nr. depends on the number of received packets until the missing one comes in */
		uint32_t endSeqNr;

		/** the number of used bytes between start and end. this value equal to (end-start) means: got everything reassembled! */
		uint32_t bytesUsed;

		/** data-storage for reassembly */
		std::vector<uint8_t> data;

	public:

		/** ctor */
		TCPReassemblyBuffer() {
			data.resize(512 * 1024);
			reset();
		}

		/**
		 * append the given payload (starting at packetsSeqNr)
		 * to the reassembly buffer starting at the streams next expected seq-nr
		 * and return payload as soon as something was (completely) reassembled
		 */
		Payload append(const uint32_t nextExpectedSeqNr, const uint32_t packetsSeqNr, const Payload p) {


			// new reassembly starting (buffer empty)?
			// set the buffer's start to the stream's last good seq-nr
			if (startSeqNr == 0) {startSeqNr = nextExpectedSeqNr;}

			// append the given payload to its designated position within the buffer
			// (depending on the last good seq-nr)
			const uint32_t offset = packetsSeqNr - startSeqNr;
			if (offset + p.length > data.size()) {throw Exception("TCPStreamBuffer full. Lost track within connection?");}
			memcpy(data.data() + offset, p.data, p.length);

			// increment the buffers end (if necessary)
			const uint32_t packetsEndSeqNr = packetsSeqNr + p.length;
			if (packetsEndSeqNr > endSeqNr) {endSeqNr = packetsEndSeqNr;}

			// update the number of used bytes within the buffer
			bytesUsed += p.length;

			debug(K_DBG_TCP_STREAM_BUFFER, "buffer used: " << bytesUsed << " from: " << startSeqNr << " to: " << endSeqNr);

			// have we got all missing packets between last good seq-nr and the end seq-nr?
			// then everything is reassembled. return the payload!
			if (bytesUsed == (endSeqNr - startSeqNr)) {
				Payload pRes(data.data(), bytesUsed);
				reset();
				return pRes;
			}

			// nothing reassembled..
			return Payload::empty();

		}

		/** reset the buffer to an empty state (no pending reassembly */
		void reset() {
			startSeqNr = 0;
			endSeqNr = 0;
			bytesUsed = 0;
		}

		/** is the buffer currenlty empty? (no pending reassembly) */
		bool isEmpty() const {
			return startSeqNr == endSeqNr;
		}

	};

}

#endif // K_NET_TCPSTREAMBUFFER_H
