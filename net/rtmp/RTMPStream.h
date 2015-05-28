#ifndef K_NET_RTMP_RTMPSTREAM_H
#define K_NET_RTMP_RTMPSTREAM_H

#include <cstdint>
#include <cassert>

#include "RTMPPacket.h"

namespace K {

	/**
	 * this struct describes the state for one RTMP stream,
	 * identified by its streamID. As RTMP sends values (size, messageID, ..)
	 * only when they changed (compared to the last value sent), we have to
	 * keep track of those values to use them when they are ommitted within
	 * a data packet
	 */
	struct RTMPStream {

		/** the stream's ID */
		uint32_t streamID;

		/** the last typeID sent within an RTMP packet */
		int lastTypeID;

		/** the last payload size sent within an RTMP packet */
		int lastPayloadSize;

		/** the last messageID sent within an RTMP packet */
		int lastMessageID;

		/** incremental number of data bytes */
		int totalDataBytes;


		/** empty ctor */
		RTMPStream() : streamID(-1), lastTypeID(-1), lastPayloadSize(-1), lastMessageID(-1), totalDataBytes(0) {;}

		/** ctor */
		RTMPStream(const uint32_t streamID) : streamID(streamID), lastTypeID(-1), lastPayloadSize(-1), lastMessageID(-1), totalDataBytes(0) {;}


		/** reset the stream to an empty state */
		void reset() {
			totalDataBytes = 0;
		}

		/** update the stream's details by using the data from the given RTMPPacket belonging to this stream */
		void update(const RTMPPacket& pkt) {

			// ensure this packet belongs to this stream
			assert(streamID == pkt.getStreamID());

			// update the typeID
			if (pkt.getTypeID() > 0) {lastTypeID = pkt.getTypeID();}

			// update the payload length
			if (pkt.getPayloadLength() >= 0) {lastPayloadSize = pkt.getPayloadLength();}

			// update the messageID
			if (pkt.getMessageID() >= 0) {lastMessageID = pkt.getMessageID();}

			// update cumulative data
			if (lastPayloadSize >= 0) {totalDataBytes += lastPayloadSize;}

		}

		/** debug output */
		friend std::ostream& operator << (std::ostream& out, const RTMPStream& s) {
			out << "RTMPStream(";
			out << "streamID: " << s.streamID;
			out << "\ttypeID: " << s.lastTypeID;
			out << "\tdataLen: " << s.lastPayloadSize;
			out << "\tmessageID: " << s.lastMessageID;
			out << "\t#bytes: " << s.totalDataBytes;
			out << ")";
			return out;
		}

	};

}

#endif // RTMPSTREAM_H
