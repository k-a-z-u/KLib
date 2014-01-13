/*
 * ServerSocket.h
 *
 *  Created on: Dec 15, 2013
 *      Author: kazu
 */

#ifndef SERVERSOCKET_H_
#define SERVERSOCKET_H_

#include "SocketTypes.h"
#include "Socket.h"

namespace K {

class ServerSocket {

public:

	/** ctor */
	ServerSocket();

	/** dtor */
	~ServerSocket();


	/** bind the socket to the given port */
	void bind(const SckPort);

	/** accept a new incoming connection request */
	Socket* accept();

	/** close the server socket */
	void close();

private:

	/** the socket handle */
	int handle;

};

}


#endif /* SERVERSOCKET_H_ */
