#ifndef K_SOCKETS_SOCKETUDP_H
#define K_SOCKETS_SOCKETUDP_H

/** the maximum allowed size for datagrams. this should be 64k for UDP over IPv4 */
#define MAX_DATAGRAM_SIZE	64*1024

#if defined(__GNUC__)
	#include <sys/socket.h>
	#include <vector>
	#include <errno.h>
	#include <netinet/in.h>
#elif defined(_WIN32)
	#include <winsock2.h>
	typedef int socklen_t;
#else
	#error "TODO"
#endif

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

		/** whether the socket is bound */
		bool bound = false;


	public:

		/** ctor */
		SocketUDP() : handle(0) {

			// Initialize Winsock
			#if defined(_WIN32)
				WSADATA wsaData;
				int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
				if (res != 0) { throw Exception("error during winsock startup"); }
			#endif

			// create socket
			handle = socket(AF_INET, SOCK_DGRAM, 0);
			if (handle == -1) {throw SocketException("error while creating socket", errno);}

		}

		/** dtor */
		~SocketUDP() {
			close();
		}

		/**
		 * @brief bind the socket to receive all datagrams sent to the given (local) port
		 * a localPort of 0 will let the OS determine a free one.
		 * @param localPort the local endpoint for datagrams sent from a remote
		 */
		void bind(const uint16_t localPort) {

			if (bound) {throw SocketException("socket already bound!");}

			// local endpoint AF_INET struct
			struct sockaddr_in srvAddr;
			memset((char*)&srvAddr, 0, sizeof(srvAddr));
			srvAddr.sin_family = AF_INET;
			srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);	// every interface
			srvAddr.sin_port = htons(localPort);

			// bind the socket to the given port
			int ret = ::bind(handle, (struct sockaddr*) &srvAddr, sizeof(srvAddr));
			if (ret < 0) {throw SocketException("error while binding socket", errno);}

			// mark as bound
			bound = true;

		}

		/** close the socket */
		void close() {

			// cleanup
			if (handle) {

                #if defined(__GNUC__)
				    ::close(handle);
                #elif defined(_WIN32)
				    ::closesocket(handle);
                #endif

				handle = 0;
			}

		}

		/** is the socket currently closed? */
		bool isClosed() const {
			return handle == 0;
		}

		/** allow to broadcast packets using this socket? */
		void allowBroadcast(const bool allow) {

			#if defined(__GNUC__)
				const int broadcastEnable = (allow) ? (1) : (0);
				const int ret = setsockopt(handle, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
				if (ret < 0) { throw SocketException("error while enabling broadcast", errno); }
			#elif defined(_WIN32)
				;
			#endif

		}

		/**
		 * @brief send a datagram to the given destination address
		 * @param data the data to send
		 * @param len the length of the data to send (max 64k!)
		 * @param addr the destination address
		 */
		void sendDatagram(const uint8_t* data, uint32_t len, const NetworkAddress& addr) {

			// sanity check
			if (!bound) {throw SocketException("bind() the socket first!");}

			// ensure max datagram size
			if (len > MAX_DATAGRAM_SIZE)	{throw SocketException("max datagram size is " + std::to_string(MAX_DATAGRAM_SIZE)+ " bytes!");}

			// ensure correct destination address
			if (!addr.isValidTargetPort())	{throw SocketException("the given destination address has no valid port number");}
			if (!addr.isValidTargetHost())	{throw SocketException("the given destination address has no valid hostname");}

			// send datagram to the given destionation
			const struct sockaddr_in& sockAddr = addr.getAsSocketAddress();
			const int options = 0;
			const int res = sendto(handle, (const char*)data, len, options, (struct sockaddr*) &sockAddr, sizeof(sockaddr));

			// check
			if (res < 0) {throw SocketException("error while sending datagram", errno);}

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

			// sanity check
			if (!bound) {throw SocketException("bind() the socket first!");}

			// ensure the target buffer may hold the largest possible datagram
			d.ensureSpace(maxSize);

			// the datagrams sender will be stored here
			struct sockaddr_in senderAddr;
			socklen_t senderLength = sizeof(senderAddr);

			// receive datagram from socket and store sender information
			int len = recvfrom(handle, (char*) d.getDataWriteable(), maxSize, flags, (struct sockaddr*) &senderAddr, &senderLength);
			if (len < 0) {throw SocketException("error while receiving datagram", errno);}

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
