#ifndef K_SOCKETS_NETWORKADDRESS_H
#define K_SOCKETS_NETWORKADDRESS_H

#include "../SocketException.h"

#include <cstring>

#if defined(__GNUC__)
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <unistd.h>
	#include <arpa/inet.h>
	#include <netdb.h>
#endif

//#include <winsock2.h>

namespace K {

	class NetworkAddress {

	public:

		/**
		 * @brief create NetworkAddress from the given socket address. This will
		 * mainly be used for incoming data frames. The ctor re-calculuates the
		 * host-ip and port-number from the given address
		 * @param address the socket address to parse
		 */
		NetworkAddress(const struct sockaddr_in& address) : port(0), sockAddr(address) {
			port = ntohs(sockAddr.sin_port);
			ipFromAddressStruct();
		}

		/**
		 * @brief create a new NetworkAddress identified by hostname and port-number
		 * @param host the host to identify (e.g. "127.0.0.1" or "google.de")
		 * @param port the 16-bit port-number to use
		 */
		NetworkAddress(const std::string& host, const uint16_t port) : port(port) {

			// convert hostname to ip
			struct hostent* he = gethostbyname( host.c_str() );
			if (!he) {throw SocketException("error while retrieving IP for hostname: '" + host + "'");}

			// sanity checks
			// https://www.cs.rutgers.edu/~pxk/417/notes/sockets/udp.html

			// build address struct
			memset( &sockAddr, 0, sizeof(sockAddr) );
			sockAddr.sin_family = AF_INET;
			//sockAddr.sin_addr = *((struct in_addr*)he->h_addr);
			sockAddr.sin_port = htons(port);
			memcpy((void*)&sockAddr.sin_addr, he->h_addr_list[0], he->h_length);

			ipFromAddressStruct();

		}


		/**
		 * @brief create a new NetworkAddress identified by the given port-number and ANY host-name
		 * @param port the 16-bit port-number to use
		 */
		NetworkAddress(const uint16_t port) : port(port) {

			// build address struct
			memset( &sockAddr, 0, sizeof(sockAddr) );
			sockAddr.sin_family = AF_INET;
			sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
			sockAddr.sin_port = htons(port);

			ipFromAddressStruct();

		}

		/**
		 * @brief create a new NetworkAddress matching ANY host and ANY port
		 * e.g. used for receiving all UDP-datagrams sent to any local port / interface
		 */
		NetworkAddress() : port(0) {

			// build address struct
			memset( &sockAddr, 0, sizeof(sockAddr) );
			sockAddr.sin_family = AF_INET;
			sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
			sockAddr.sin_port = htons(0);

			ipFromAddressStruct();

		}


		/** copy ctor */
		NetworkAddress(const NetworkAddress& other) :
			port(other.port), sockAddr(other.sockAddr) {

			ipFromAddressStruct();

		}

		/** get a NetworkAddress to broadcast to the given port */
		static NetworkAddress getForBroadcast(const uint16_t port) {
			NetworkAddress adr;
			memset(&adr.sockAddr, 0, sizeof(adr.sockAddr));
			adr.sockAddr.sin_family = AF_INET;
			adr.sockAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
			adr.sockAddr.sin_port = htons(port);
			adr.port = port;
			return adr;
		}

		/** assignment operator */
		void operator = (const NetworkAddress& other) {
			this->hostName = other.hostName;
			this->hostIP = other.hostIP;
			this->port = other.port;
			this->sockAddr = other.sockAddr;
		}


		/** dtor */
		~NetworkAddress() {
			;
		}



		/** get the network address as sockaddr_in struct */
		const struct sockaddr_in& getAsSocketAddress() const {
			return sockAddr;
		}

		/** get the remote's port */
		uint16_t getPort() const {
			return port;
		}

		/** get the host's IP as string */
		const std::string& getHostIP() const {
			return hostIP;
		}

		/** get the host's name as string */
		const std::string& getHostName() {
			if (hostName.empty()) { hostNameFromAddressStruct(); }
			return hostName;
		}



		/** is the internal port valid for an outbound packet? */
		bool isValidTargetPort() const {
			return port != 0;
		}

		/** is the internal ip valid for an outbound packet? */
		bool isValidTargetHost() const {
			return sockAddr.sin_addr.s_addr != htonl(INADDR_ANY);
		}



		/** check whether both NetworkAddresses are equal */
		bool operator == (const NetworkAddress& other) const {
			return memcmp( &this->sockAddr, &other.sockAddr, sizeof(sockAddr)) == 0;
		}



	private:

		/** fill the hostIP string from the address struct */
		void ipFromAddressStruct() {

			uint32_t ip = sockAddr.sin_addr.s_addr;
			hostIP =	std::to_string( (ip>> 0) & 0xFF ) + "." +
						std::to_string( (ip>> 8) & 0xFF ) + "." +
						std::to_string( (ip>>16) & 0xFF ) + "." +
						std::to_string( (ip>>24) & 0xFF );

		}

		/** fill the hostName string via reverse-lookup of the address struct */
		void hostNameFromAddressStruct() {

			char hostNameBuf[128];
			const int flags = 0;

			// reverse lookup
#if defined(__GNUC__)
			getnameinfo( (struct sockaddr*) &sockAddr, sizeof(sockAddr), hostNameBuf, 128, nullptr, 0, flags );
#endif
			this->hostName = std::string(hostNameBuf);

		}



	private:

		/** the host's ip as string */
		std::string hostIP;

		/** the host's name as string */
		std::string hostName;

		/** the port-number from the ctor */
		uint16_t port;

		/** the resulting socket-address-struct */
		struct sockaddr_in sockAddr;

	};

}

#endif // K_SOCKETS_NETWORKADDRESS_H
