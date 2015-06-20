#ifndef K_NETWORK_PACKETTCP_H
#define K_NETWORK_PACKETTCP_H

namespace K {

	#define TCP_FLAG_URG	(1 << 5)
	#define TCP_FLAG_ACK	(1 << 4)
	#define TCP_FLAG_PSH	(1 << 3)
	#define TCP_FLAG_RST	(1 << 2)
	#define TCP_FLAG_SYN	(1 << 1)
	#define TCP_FLAG_FIN	(1 << 0)


	/** structural representation of a TCP packet */
	struct PacketTCP {

		struct __attribute__ ((packed)) Data {
			uint8_t srcPort[2];
			uint8_t dstPort[2];
			uint8_t seqNr[4];
			uint32_t ackNr;
			uint8_t offsetAndFlags[2];
		};

		/** the packets data */
		Data* d;

		/** the packets total length (header + payload) */
		uint32_t len;


	private:

		/** hidden ctor */
		PacketTCP(uint8_t* data, uint32_t len) : d((Data*)data), len(len) {;}

	public:

		/** empty ctor */
		PacketTCP() {;}

		/** helper method */
		static PacketTCP wrap(uint8_t* data, const uint32_t len) {
			return PacketTCP(data, len);
		}

		/** get the tcp sequence number */
		uint32_t getSeqNumber() const {return (d->seqNr[0] << 24) | (d->seqNr[1] << 16) | (d->seqNr[2] << 8) | (d->seqNr[3] << 0);}

		/** set the tcp sequence number */
		void setSeqNumber(uint32_t seq) {
			d->seqNr[0] = seq >> 24;
			d->seqNr[1] = seq >> 16;
			d->seqNr[2] = seq >> 8;
			d->seqNr[3] = seq >> 0;
		}

		/** is the URG flag set? */
		bool isUrgent() const {return d->offsetAndFlags[1] & TCP_FLAG_URG;}

		/** is the ACK flag set? */
		bool isAck() const {return d->offsetAndFlags[1] & TCP_FLAG_ACK;}

		/** is the PSH flag set? */
		bool isPush() const {return d->offsetAndFlags[1] & TCP_FLAG_PSH;}

		/** is the RST flag set? */
		bool isReset() const {return d->offsetAndFlags[1] & TCP_FLAG_RST;}

		/** is the SYN flag set? */
		bool isSyn() const {return d->offsetAndFlags[1] & TCP_FLAG_SYN;}

		/** is the FIN flag set? */
		bool isFin() const {return d->offsetAndFlags[1] & TCP_FLAG_FIN;}

		/** get all flags as 6-bit value. (use with TCP_FLAG_xxx) */
		uint8_t getFlags() const {return d->offsetAndFlags[1] & 0b00111111;}

		/** set the flags for this TCP packet */
		void setFlags(bool urg, bool ack, bool psh, bool rst, bool syn, bool fin) {
			d->offsetAndFlags[1] &= ~0b00111111;
			d->offsetAndFlags[1] |= (urg) ? (1 << 5) : (0);
			d->offsetAndFlags[1] |= (ack) ? (1 << 4) : (0);
			d->offsetAndFlags[1] |= (psh) ? (1 << 3) : (0);
			d->offsetAndFlags[1] |= (rst) ? (1 << 2) : (0);
			d->offsetAndFlags[1] |= (syn) ? (1 << 1) : (0);
			d->offsetAndFlags[1] |= (fin) ? (1 << 0) : (0);
		}

		/** get the source port */
		uint16_t getSrcPort() const {return d->srcPort[0] << 8 | d->srcPort[1];}

		/** get the destination port */
		uint16_t getDstPort() const {return d->dstPort[0] << 8 | d->dstPort[1];}

		/** set the source port */
		void setSrcPort(uint16_t port) {d->srcPort[0] = port >> 8; d->srcPort[1] = port & 0xFF;}

		/** set the destination port */
		void setDstPort(uint16_t port) {d->dstPort[0] = port >> 8; d->dstPort[1] = port & 0xFF;}

		/** get the length of the TCP header */
		uint8_t getHeaderLength() const {return (d->offsetAndFlags[0] >> 4) << 2;}

		/** set the length of the TCP header */
		void setHeaderLength(uint8_t length) {
			_assertTrue(length >= 20, "TCP's header length must be at least 20!");
			_assertTrue((length % 4) == 0, "TCP's header length must be a multiple of 4!");
			d->offsetAndFlags[0] &= ~0b11110000;
			d->offsetAndFlags[0] |= ((length >> 2) << 4);
		}

		/** get the packet's payload length */
		uint32_t getPayloadLength() const {
			return len - getHeaderLength();
		}

		/** get the packet's total length (header+payload) */
		uint32_t getLength() const {
			return len;
		}

		/** get the payload of this TCP packet */
		Payload getPayload() const {
			return Payload( (uint8_t*) d + getHeaderLength(), getPayloadLength());
		}

		/** get the complete packet (header+payload) as raw pointer */
		const uint8_t* getRaw() const {
			return (uint8_t*) d;
		}

		/** dump info of this packet */
		void dump() {
			std::cout << "\t\tTCP" << std::endl;
			std::cout << "\t\t- src port: " << getSrcPort() << std::endl;
			std::cout << "\t\t- dst port: " << getDstPort() << std::endl;
			std::cout << "\t\t- header-len: " << (int) getHeaderLength() << std::endl;
			std::cout << "\t\t- FLAGS: ";
			if (isUrgent()) {std::cout << "URG ";}
			if (isAck()) {std::cout << "ACK ";}
			if (isPush()) {std::cout << "PSH ";}
			if (isReset()) {std::cout << "RST ";}
			if (isSyn()) {std::cout << "SYN ";}
			if (isFin()) {std::cout << "FIN ";}
			std::cout << std::endl;
		}

	};

}

#endif // K_NETWORK_PACKETTCP_H
