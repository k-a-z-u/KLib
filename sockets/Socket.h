/*
 * Socket.h
 *
 *  Created on: Dec 15, 2013
 *      Author: kazu
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include "SocketTypes.h"
#include "SocketException.h"

#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "../../klib/sockets/SocketInputStream.h"
#include "../../klib/sockets/SocketOutputStream.h"

namespace K {

class ServerSocket;

/**
 * socket for network IO
 */
class Socket {

public:

	/** ctor */
	Socket();

	/** dtor */
	~Socket();


	/** connect to the given host / port */
	void connect(const SckHost& host, const SckPort port);

	/** close the socket */
	void close();


	/** initiate a SSL handshake to be answered by the other side */
	void startServerSSL();

	/** answer a SSL handshake initiated by the other side */
	void startClientSSL();


	/** get an InputStream to read from this socket */
	SocketInputStream getInputStream();

	/** get an OutputStream to write to this socket */
	SocketOutputStream getOutputStream();


protected:

	/** the read / write methods can be accessed using streams */

	friend class SocketInputStream;
	friend class SocketOutputStream;

	/** write the given bytes */
	void write(uint8_t* data, unsigned int len);

	/** read the given number of bytes */
	int read(uint8_t* data, unsigned int len);


private:

	friend class ServerSocket;

	/** create socket from existing handle */
	Socket(int handle);



	/** the socket's handle */
	int handle;

	/** SSL specifics */
	struct SSLs {
		SSL* handle;
		SSL_CTX* context;
		bool enabled;
		SSLs() : handle(nullptr), context(nullptr), enabled(false) {;}
	} ssl;

};

}


#endif /* SOCKET_H_ */
