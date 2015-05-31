#ifndef K_NET_RTMP_RTMPPACKET_H
#define K_NET_RTMP_RTMPPACKET_H

#include <cstdint>

#include "RTMPTypes.h"
#include "../raw/protocols/Payload.h"

namespace K {

	/**
	 * represents (wraps) one packet within an RTMP transmission
	 *
	 * depending on the chunkType (first byte) some of the hereafter
	 * following fields may be omitted. If so, usually their last
	 * known value is used instead. e.g. if the payload-size is not
	 * transmitted, the payload has the size of the last payload-size
	 * that was transmitted. this is, to decrease the protocol overhead.
	 *
	 */
	class RTMPPacket {

		uint8_t chunkType;
		uint32_t streamID;

		int32_t ts = -1;

		int32_t payloadLength = -1;
		int32_t typeID = -1;

		int32_t messageID = -1;

		const uint8_t* data;
		const uint32_t length;


	public:

		/** ctor */
		RTMPPacket(K::Payload p) : RTMPPacket(p.data, p.length) {
			;
		}

		/** ctor */
		RTMPPacket(const uint8_t* data, const uint32_t len) : data(data), length(len) {

			// get the stream's chunk type
			chunkType = data[0] >> 6;

			// get the stream's ID (1-3 Bytes)
			streamID = data[0] & 0b00111111;
			int o = 0;
			if (streamID == 0) {o = 1; streamID = (data[1]);}							// streamID NOT YET CORRECT!
			if (streamID == 1) {o = 2; streamID = (data[1] << 8) | (data[2] << 0);}		// streamID NOT YET CORRECT!

			// depending on the chunk type, some parts of the packet are missing
			// type 0: everything, type 1: messageID missing, type 2, ...
			switch (chunkType) {
				case 0:
					messageID = (data[o+8] << 0) | (data[o+9] << 8) | (data[o+10] << 16) | (data[o+11] << 24);
				case 1:
					typeID = data[o+7];
					payloadLength =	(data[o+4] << 16) | (data[o+5] << 8) | (data[o+6] << 0);
				case 2:
					ts =	(data[o+1] << 16) | (data[o+2] << 8) | (data[o+3] << 0);
				case 3:
					break;
			}



		}

		/** debug output */
		friend std::ostream& operator << (std::ostream& out, const RTMPPacket& p) {

			out << "RTMPPacket(";
			out << "streamID: "		<< p.streamID;

			out << "\ttypeID: ";
			if (p.typeID == -1) {out << "-";} else {out << p.typeID;}

			out << "\tdataLen: ";
			if (p.payloadLength == -1) {out << "-";} else {out << (p.payloadLength);}

			out << "\tmessageID: ";
			if (p.messageID == -1) {out << "-";} else {out << p.messageID;}

			out << ")";
			return out;

		}

		/** is this a chunkType 0 (all fields present) header? */
		bool isFullHeader() {
			return chunkType == 0;
		}

		/** get the length of the payload (if given by the server) [-1 if not given] */
		int getPayloadLength() const {
			return payloadLength;
		}

		/** get the packet's messageID (if given by the server) [-1 if not given] */
		int getMessageID() const {
			return messageID;
		}

		/** get the streamID this packet belongs to */
		uint32_t getStreamID() const {
			return streamID;
		}

		/** get the type of message */
		int getTypeID() const {
			return typeID;
		}

		/** get the length of the RTMP header */
		int getHeaderLength() const {
			switch(chunkType) {
				case 0: return 12;
				case 1: return 8;
				case 2: return 4;
				case 3: return 1;
				default: throw "error";
			}
		}

		/** get a pointer to the start of the payload */
		const uint8_t* getPayload() const {
			return this->data + getHeaderLength();
		}

	};

}

#endif // RTMPPACKET_H
