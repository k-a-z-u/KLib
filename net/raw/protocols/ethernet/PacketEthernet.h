#ifndef K_NETWORK_PACKETETHERNET_H
#define K_NETWORK_PACKETETHERNET_H

#include "../Payload.h"

namespace K {

	/** (excerpt of) supported payloads an ethernet frame might have */
	enum PacketEthernetType {

		IPV4 = 0x0800,

		ARP = 0x0806,

		WAKE_ON_LAN = 0x0842,

		IPV6 = 0x86DD,

	};


	/** represents an ethernet packet */
	struct PacketEthernet {


		/** the MAC-address of the sender */
		uint8_t srcMAC[6];

		/** the MAC-address of the recipient */
		uint8_t dstMAC[6];

		/** the payload's type */
		uint8_t type[2];


		/** helper method */
		static PacketEthernet* wrap(uint8_t* data, uint32_t len) {
			return (PacketEthernet*) data;
		}


		/** set the type of payload this ethernet packet contains */
		void setPayloadType(const PacketEthernetType newType) {type[0] = newType >> 8; type[1] = newType & 0xFF;}

		/** get the type of payload this ethernet packet contains */
		PacketEthernetType getPayloadType() const {return (PacketEthernetType) ((type[0] << 8) | (type[1] << 0));}


		/** get the length of the ethernet header (fixed) */
		uint8_t getHeaderLength() const {return 14;}

		/** get the frame's payload */
		uint8_t* getPayload() {
			return ((uint8_t*) this) + getHeaderLength();
		}

		/** get the header of this Ethernet packet */
		Payload getHeader() {return Payload( (uint8_t*) this, getHeaderLength() );}

//		/** get the total length of this packet including payload */
//		uint32_t getTotalLength() const {
//			const uint32_t length = getHeaderLength();
//			if (containsIPv4()) {length += getIPv4()->getTotalLength();}
//			return length;
//		}

		/** dump info of this packet */
		void dump() {
			std::cout << "ethernet" << std::endl;
			std::cout << "- src-mac: ";
			for (int i = 0; i < 6; ++i) {std::cout << std::hex << srcMAC[i];}
			std::cout << "- dst-mac: ";
			for (int i = 0; i < 6; ++i) {std::cout << std::hex << dstMAC[i];}
		}

	};

}

#endif // K_NETWORK_PACKETETHERNET_H
