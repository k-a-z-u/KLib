#ifndef K_SOCKETS_SOCKETUDP_H
#define K_SOCKETS_SOCKETUDP_H

#include <sys/socket.h>
#include <vector>

#include "SocketException.h"
#include "address/NetworkAddress.h"

#include <netinet/in.h>

#include "datagram/DefaultDatagram.h"

namespace K {

	/**
	 * @brief this class represents a socket capable of sending and
	 * receiving UDP datagrams.
	 */
	class SocketUDP {

		/** the maximum allowed size for datagrams. this should be 64k for UDP over IPv4 */
		static const int MAX_DATAGRAM_SIZE = 64*1024;


	public:

		/** ctor */
		SocketUDP() : handle(0) {
			;
		}

		/** dtor */
		~SocketUDP() {

			// cleanup
			if (handle) {
				::close(handle);
				handle = 0;
			}

		}

		/**
		 * @brief bind the socket to receive all datagrams sent to the given (local) port
		 * @param localPort the local endpoint for datagrams sent from a remote
		 */
		void bind(uint16_t localPort) {

			// local endpoint struct
			struct sockaddr_in srvAddr;
			srvAddr.sin_family = AF_INET;
			srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
			srvAddr.sin_port = htons(localPort);

			// create socket
			handle = socket(AF_INET, SOCK_DGRAM, 0);
			if (handle == -1) {throw SocketException("error while creating socket");}

			// bind the socket to the given port
			int ret = ::bind(handle, (struct sockaddr*) &srvAddr, sizeof(struct sockaddr));
			if (ret < 0) {throw new SocketException("error while binding socket");}

		}

		/**
		 * @brief send a datagram to the given destination address
		 * @param data the data to send
		 * @param len the length of the data to send (max 64k!)
		 * @param addr the destination address
		 */
		void sendDatagram(const uint8_t* data, uint32_t len, const NetworkAddress& addr) {

			// ensure max datagram size
			if (len > MAX_DATAGRAM_SIZE)	{throw SocketException("max datagram size is " + std::to_string(MAX_DATAGRAM_SIZE)+ " bytes!");}

			// ensure correct destination address
			if (!addr.isValidTargetPort())	{throw SocketException("the given destination address has no valid port number");}
			if (!addr.isValidTargetHost())	{throw SocketException("the given destination address has no valid hostname");}

			// send datagram to the given destionation
			const struct sockaddr_in& sockAddr = addr.getAsSocketAddress();
			const int options = 0;
			sendto(handle, data, len, options, (struct sockaddr*) &sockAddr, sizeof(struct sockaddr));

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

			const int flags = 0;
			const int maxSize = MAX_DATAGRAM_SIZE;

			// ensure the target buffer may hold the largest possible datagram
			d.ensureSpace(maxSize);

			// the datagrams sender will be stored here
			struct sockaddr_in senderAddr;
			socklen_t senderLength = sizeof(senderAddr);

			// receive datagram from socket and store sender information
			ssize_t len = recvfrom(handle, d.getDataWriteable(), maxSize, flags, (struct sockaddr*) &senderAddr, &senderLength);
			if (len < 0) {throw new SocketException("error while receiving datagram");}

			// store senders network-address in the datagram
			NetworkAddress na(senderAddr);
			d.setAddress(na);

			// inform buffer about the actual size of the datagram
			d.setLength( (uint32_t) len );

		}



	private:

		/** the socket handle */
		int handle;

		/** the local address the socket is bound to */
		NetworkAddress localAddress;

	};

}

#endif // K_SOCKETS_UDPSOCKET_H
