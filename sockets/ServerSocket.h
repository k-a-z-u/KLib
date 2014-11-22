/*
 * ServerSocket.h
 *
 *  Created on: Dec 15, 2013
 *      Author: kazu
 */

#ifndef SERVERSOCKET_H_
#define SERVERSOCKET_H_

#include "SocketException.h"
#include "SocketTypes.h"
#include "Socket.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

namespace K {

	class ServerSocket {

	public:

		/** ctor */
		ServerSocket() : handle(0) {

		}

		/** dtor */
		~ServerSocket() {
			close();
		}


		/** bind the socket to the given port */
		void bind(const SckPort port) {

			// cleanup
			if (handle) {::close(handle); handle = 0;}

			// create socket
			handle = socket(AF_INET, SOCK_STREAM, 0);
			if (!handle) {throw SocketException("error creating server socket");}

			// bind to the given port
			struct sockaddr_in srvAddr;
			srvAddr.sin_family = AF_INET;
			srvAddr.sin_addr.s_addr = INADDR_ANY;
			srvAddr.sin_port = htons(port);
			int ret = ::bind(handle, (struct sockaddr*) &srvAddr, sizeof(srvAddr));
			if (ret < 0) {throw SocketException("error while binding server socket");}

			// start listening
			::listen(handle, 5);

		}

		/** accept a new incoming connection request */
		Socket* accept() {

			struct sockaddr_in cliAddr;
			socklen_t addrLen = sizeof(cliAddr);

			int newHandle = ::accept(handle, (struct sockaddr*) &cliAddr, &addrLen);
			if (newHandle < 0) {throw SocketException("error while accepting client connection");}

			return new Socket(newHandle);

		}

		/** close the server socket */
		void close() {

			if (handle) {
				::shutdown(handle, 2);
				::close(handle);
				handle = 0;
			}

		}

	private:

		/** the socket handle */
		int handle;

	};

}


#endif /* SERVERSOCKET_H_ */
