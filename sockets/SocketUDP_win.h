#ifndef SOCKETUDP_WIN_H
#define SOCKETUDP_WIN_H

#include <vector>

#include "SocketException.h"
#include "address/NetworkAddress.h"

#include "datagram/DefaultDatagram.h"

namespace K {

	/**
	 * @brief this class represents a socket capable of sending and
	 * receiving UDP datagrams.
	 */
	class SocketUDP {

	private:

		/** the maximum allowed size for datagrams. this should be 64k for UDP over IPv4 */
		static const int MAX_DATAGRAM_SIZE = 64*1024;



	public:

		/** ctor */
		SocketUDP()  {


		}

		/** dtor */
		~SocketUDP() {



		}

		/**
		 * @brief bind the socket to receive all datagrams sent to the given (local) port
		 * a localPort of 0 will let the OS determine a free one.
		 * @param localPort the local endpoint for datagrams sent from a remote
		 */
		void bind(const uint16_t localPort) {


		}

		/** allow to broadcast packets using this socket? */
		void allowBroadcast(const bool allow) {

		}

		/**
		 * @brief send a datagram to the given destination address
		 * @param data the data to send
		 * @param len the length of the data to send (max 64k!)
		 * @param addr the destination address
		 */
		void sendDatagram(const uint8_t* data, uint32_t len, const NetworkAddress& addr) {


		}

		/**
		 * @brief send a datagram to the given destination address
		 * @param data the data to send (max 64k!)
		 * @param addr the destination address
		 */
		void sendDatagram(const std::vector<uint8_t>& data, const NetworkAddress& addr) {
			sendDatagram(data.data(), (uint32_t) data.size(), addr);
		}

		/**
		 * @brief send the given datagram to its internal destination address
		 * @param d the datagram to send
		 */
		void sendDatagram(const Datagram& d) {
			sendDatagram(d.getData(), d.getLength(), d.getAddress());
		}


		/**
		 * @brief convenience function for receiveDatagram(d) which returns a new
		 * datagram instead of reusing an existing one.
		 * @return a newly created datagram holding the received message
		 */
		DefaultDatagram receiveDatagram() {
			DefaultDatagram dd;
			receiveDatagram(dd);
			return dd;
		}

		/**
		 * @brief this method will block until a new datagram is received on the bound
		 * port of the underlying socket. the received data will be stored within the
		 * given datagram
		 * @param d the buffer to store the received datagram to
		 */
		void receiveDatagram(Datagram& d) {



		}


	};

}


#endif // SOCKETUDP_WIN_H
