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

	class TCPReassembler;

	class TCPReassemblerListener {

	public:

		/** the reassembler detected a new connection */
		virtual void onTCPNewConnection(TCPReassembler* tr, TCPConnection* con) = 0;

		/** the reassembler detected a new stream on a connection */
		virtual void onTCPNewConnectionStream(TCPReassembler* tr, TCPConnection* con, TCPStream* s) = 0;

		/** closed an existing TCP-connection */
		virtual void onTCPClosedConnection(TCPReassembler* tr, TCPConnection* con) = 0;

		/** received new packets on a tcp connection */
		virtual void onTCPData(TCPReassembler* tr, TCPConnection* con, TCPStream* s, std::vector<PacketTCP> packets) = 0;

	};

	/**
	 * this class will perform tcp stream reassembly of the individual
	 * packets passed into it
	 * packets are assigned to connections and reordered if needed
	 */

	class TCPReassembler {

	private:

		/** set to keep track of all connections */
		std::unordered_map<TCPConID, TCPConnection*> connections;

		/** the event listener */
		TCPReassemblerListener* listener;

		/** whether to track already running connections or just new ones (after SYN) */
		bool trackRunningConnections;

	public:

		/** ctor */
		TCPReassembler() : listener(nullptr), trackRunningConnections(false) {
			;
		}

		/** set whether to also track already running connections, or just new ones (after SYN) */
		void setTrackRunningConnections(const bool track) {
			this->trackRunningConnections = track;
		}

		/** set the listener */
		void setListener(TCPReassemblerListener* listener) {
			this->listener = listener;
		}

		/** add a new packet to the reassembler */
		void add(const PacketTCP& pkt) {

			// get the connection for this packet
			TCPConnection* con = connection(pkt);
			if (!con) {return;}

			// get the stream (direction) this packet belongs to
			TCPStream* stream = con->getStreamForDST(pkt.getDstPort());

			// add the packet to the stream
			std::vector<PacketTCP> reassembled = stream->add(pkt);

			// something reassembled?
			if (!reassembled.empty()) {
				if (listener) {listener->onTCPData(this, con, stream, reassembled);}
			}

		}

	private:

		/** get, create, close, whatever the connection beloging to this packet. may return null */
		TCPConnection* connection(const PacketTCP& pkt) {

			if (pkt.isSyn()) {

				// SYN-packet -> this is a new connection
				return createConnection(pkt);

			} else if (pkt.isFin()) {

				deleteConnection(pkt);
				return nullptr;

			} else if (trackRunningConnections) {

				// allows listening to allready running connections (no explicit SYN needed)
				return getOrCreateConnection(pkt);

			} else {

				// try to get the corresponding connection (if exists)
				return getConnection(pkt);

			}

		}

		/** delete the connection for this packet (e.g. if the packet is a FIN) */
		void deleteConnection(const PacketTCP& pkt) {

			const TCPConID id(pkt);
			TCPConnection* con = getConnection(pkt);
			if (!con) {return;}

			// inform listeners
			if (listener) {listener->onTCPClosedConnection(this, con);}

			// cleanup
			connections.erase(id);
			delete con;

		}

		/** get the TCP-Connection (if exists) beloging to this packet and update the connection (if necessary) */
		TCPConnection* getConnection(const PacketTCP& pkt) {

			const TCPConID id(pkt);
			auto it = connections.find(id);
			if (it == connections.end()) {return nullptr;}

			TCPConnection* con = (it->second);

			// attach second half (stream) of a connection?
			if (con->out == nullptr && !con->in->getDstPort() != pkt.getDstPort()) {

				// create the other stream direction within this connection
				con->out = new TCPStream(pkt);

				// inform listeners
				if (listener) {listener->onTCPNewConnectionStream(this, con, con->out);}

			}

			// done
			return con;

		}

		/** create a new TCP-connection based on the given packet */
		TCPConnection* createConnection(const PacketTCP& pkt) {

			// create the first stream within this connection
			TCPStream* in = new TCPStream(pkt);		// given by the packet
			TCPStream* out = nullptr;				// yet unknown. given by upcoming packets

			// create and add the new connection
			const TCPConID id(pkt);
			TCPConnection* con = new TCPConnection(in, out);
			connections[id] = con;

			// inform listeners
			if (listener) {
				listener->onTCPNewConnection(this, con);
				listener->onTCPNewConnectionStream(this, con, con->in);
			}

			// done
			return con;

		}

		/** get or create a tcp connection for this packet */
		TCPConnection* getOrCreateConnection(const PacketTCP& pkt) {

			const TCPConID id(pkt);

			// no such connection yet present?
			if (connections.find(id) == connections.end()) {
				return createConnection(pkt);
			} else {
				return getConnection(pkt);
			}

		}

		/** get the number of currently active connections */
		int getNumActiveConnections() const {return connections.size();}

	};

}





#endif // K_NET_TCPREASSEMBLER_H
