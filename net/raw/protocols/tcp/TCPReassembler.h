#ifndef K_NET_TCPREASSEMBLER_H
#define K_NET_TCPREASSEMBLER_H

#include "PacketTCP.h"
#include "TCPConnection.h"

#include <unordered_map>


namespace K {

	struct TCPConID {
		uint16_t srcPort;
		uint16_t dstPort;
		uint32_t getHash() const {return srcPort+dstPort;}
		bool operator == (const TCPConID o) const {
			return	(o.srcPort == srcPort && o.dstPort == dstPort) ||
					(o.dstPort == srcPort && o.srcPort == dstPort);
		}
		TCPConID() : srcPort(0), dstPort(0) {;}
		TCPConID(const PacketTCP& tcp) : srcPort(tcp.getSrcPort()), dstPort(tcp.getDstPort()) {;}
	};

}

namespace std {
	template<> struct hash<K::TCPConID> {
		size_t operator()(const K::TCPConID& c) const {return c.getHash();}
	};
}

namespace K {

	/**
	 * this class will perform tcp stream reassembly of the individual
	 * packets passed into it
	 * packets are assigned to connections and reordered if needed
	 */

	class TCPReassembler {

	private:

		/** set to keep track of all connections */
		std::unordered_map<TCPConID, TCPConnection> connections;

	public:

		/** add a new packet to the reassembler */
		void add(const PacketTCP& pkt) {

			TCPConnection& con = getOrCreateConnection(pkt);

		}

		/** get or create a tcp connection for this packet */
		TCPConnection& getOrCreateConnection(const PacketTCP& pkt) {
			const TCPConID id(pkt);
			if (connections.find(id) == connections.end()) {
				connections[id] = TCPConnection();
			}
			return connections[id];
		}

	};

}





#endif // K_NET_TCPREASSEMBLER_H
