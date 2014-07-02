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

#include "SocketInputStream.h"
#include "SocketOutputStream.h"

#include "address/NetworkAddress.h"

#ifdef WITH_SSL
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

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


	/** connect to the given NetworkAddress */
	void connect(const NetworkAddress& target);

	/** close the socket */
	void close();

#ifdef WITH_SSL

	/** initiate a SSL handshake to be answered by the other side */
	void startServerSSL();

	/** answer a SSL handshake initiated by the other side */
	void startClientSSL();

#endif


	/** get an InputStream to read from this socket */
	SocketInputStream getInputStream();

	/** get an OutputStream to write to this socket */
	SocketOutputStream getOutputStream();


protected:

	/** the read / write methods can be accessed using streams */

	friend class SocketInputStream;
	friend class SocketOutputStream;

	/** write the given bytes */
	void write(const uint8_t* data, unsigned int len);

	/** read the given number of bytes */
	int read(uint8_t* data, unsigned int len);


private:

	friend class ServerSocket;

	/** create socket from existing handle */
	Socket(int handle);



	/** the socket's handle */
	int handle;

#ifdef WITH_SSL
	/** SSL specifics */
	struct SSLs {
		SSL* handle;
		SSL_CTX* context;
		bool enabled;
		SSLs() : handle(nullptr), context(nullptr), enabled(false) {;}
	} ssl;
#endif


};

}


#endif /* SOCKET_H_ */
