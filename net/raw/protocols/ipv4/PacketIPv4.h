#ifndef K_NETWORK_PACKETIPV4_H
#define K_NETWORK_PACKETIPV4_H

#include <cstdint>
#include "../Payload.h"
#include "../../../../Assertions.h"

namespace K {


	/** (excerpt of) supported payloads within IPv4 packets */
	enum PacketIPv4Type {

		TCP = 6,

		UDP = 17,

	};

	/** structural representation of an IPv4 packet */
	struct __attribute__((__packed__)) PacketIPv4 {

		/** the header's length (in multiples of 4 bytes) */
		uint8_t headerLength : 4;

		/** the protocol version, usually "4" */
		uint8_t version : 4;

		/** special types of payload.. */
		uint8_t typeOfService;

		/** packets length: payload AND header! */
		uint8_t length[2];

		/** datagram ID (used for reassembly of fragmented packets) */
		uint8_t id[2];

		/** the flags set for this packet and offset (if fragmented) */
		uint8_t flagsAndOffset[2];

		/** the packet's time to live */
		uint8_t ttl;

		/** the contained payload's type */
		uint8_t protocol;

		/** the packet's checksum */
		uint16_t checkSum;

		/** sender's IP */
		uint8_t srcIP[4];

		/** recipient's IP */
		uint8_t dstIP[4];


		/** helper method */
		static PacketIPv4* wrap(uint8_t* data, const uint32_t len) {
			return (PacketIPv4*) data;
		}


		/** get the IPv4 ID of this packet */
		uint16_t getID() const {return id[0] << 8 | id[1];}

		/** get the length of the IPv4 header (in bytes) */
		uint8_t getHeaderLength() const {return (headerLength << 2);}


		/** set the length of the IPv4 header (in bytes) */
		void setHeaderLength(uint8_t length) {
			_assertTrue(length >= 20, "IPv4's header length must be at least 20!");
			_assertTrue((length % 4) == 0, "IPv4's header length must be a multiple of 4!");
			headerLength = (length >> 2);
		}

		/** get the source-IP as uint32 */
		uint32_t getSrcIP() const {return srcIP[0] << 24 | srcIP[1] << 16 | srcIP[2] << 8 | srcIP[3];}

		/** get the source-IP as uint32 */
		uint32_t getDstIP() const {return dstIP[0] << 24 | dstIP[1] << 16 | dstIP[2] << 8 | dstIP[3];}


		/** get the length of this IPv4 packet INCLUDING the header */
		uint16_t getTotalLength() const {return (length[0] << 8) | (length[1] << 0);}

		/** set the total length of this IPv4 packet INCLUDING the header */
		void setTotalLength(const uint16_t totalLength) {length[0] = totalLength >> 8; length[1] = totalLength & 0xFF;}


		/** are more fragments of this packet on their way? */
		bool moreFragements() const {return (flagsAndOffset[0] & (1<<5));}

		/** set whether there will be more fragments for this IPv4 packet */
		void setMoreFragments(const bool more) {flagsAndOffset[0] &= ~(1<<5); flagsAndOffset[0] |= (more) ? (1<<5) : (0);}


		/** get the offset of this IPv4 packet (if fragmented) */
		uint16_t getOffset() const {return ((flagsAndOffset[0] & 0b00011111) << 8 | flagsAndOffset[1]) * 8;}

		/** set the offset of this IPv4 packet (if fragmented) */
		void setOffset(uint16_t offset) {
			if (offset % 8 != 0) {throw Exception("IPv4 offset must be a multiple of 8!");}
			offset /= 8;
			flagsAndOffset[0] = flagsAndOffset[0] & 0b11100000;
			flagsAndOffset[1] = 0;
			flagsAndOffset[0] |= (offset >> 8) & 0b00011111;
			flagsAndOffset[1] = (offset >> 0) & 0b11111111;
		}

		/** get the length of the payload */
		uint32_t getPayloadLength() const {return getTotalLength() - getHeaderLength();}


		/** get the header of this IPv4 packet */
		Payload getHeader() {return Payload( (uint8_t*) this, getHeaderLength() );}

		/** get the payload of this packet */
		Payload getPayload() {return Payload( (uint8_t*) this + getHeaderLength(), getPayloadLength());}


		/** get the type of payload this IPv4 packet contains */
		PacketIPv4Type getPayloadType() const {return (PacketIPv4Type) protocol;}

		/** set the type of the inner protocol. (e.g. TCP, UDP) */
		void setPayloadType(const PacketIPv4Type payloadType) {protocol = payloadType;}


		/** dump info of this packet */
		void dump() {
			std::cout << "\tIPv4" << std::endl;
			std::cout << "\t- src-IP: " << (int)srcIP[0] << "." << (int)srcIP[1] << "." << (int)srcIP[2] << "." << (int)srcIP[3] << std::endl;
			std::cout << "\t- dst-IP: " << (int)dstIP[0] << "." << (int)dstIP[1] << "." << (int)dstIP[2] << "." << (int)dstIP[3] << std::endl;
			std::cout << "\t- header size: " << (int)getHeaderLength() << std::endl;
			std::cout << "\t- payload size: " << getPayloadLength() << std::endl;
			std::cout << "\t- offset: " << getOffset() << std::endl;
			//if (containsTCP()) {getTCP()->dump();}
		}

	};

}

#endif // K_NETWORK_PACKETIPV4_H
