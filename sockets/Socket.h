/*
 * Socket.h
 *
 *  Created on: Dec 15, 2013
 *      Author: kazu
 */

#ifndef K_SOCKETS_SOCKET_H_
#define K_SOCKETS_SOCKET_H_

#include "SocketTypes.h"
#include "SocketException.h"

#include "address/NetworkAddress.h"

#ifdef WITH_SSL
	#include <openssl/rand.h>
	#include <openssl/ssl.h>
	#include <openssl/err.h>
	#include "SSLSocketException.h"
#endif

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

//http://svn.netlabs.org/repos/ports/hermes/trunk/src/Socket.cpp
//	private key:		openssl genrsa -out privkey.pem 2048
//	signing-request:	openssl req -new -key privkey.pem -out server.csr
//	certificate:		openssl x509 -req -days 365 -in server.csr -signkey privkey.pem -out server.crt


namespace K {

	class ServerSocket;
	class SocketInputStream;
	class SocketOutputStream;

	/**
	 * TCP socket implementation for network IO.
	 * this socket is either created by a server that accepted an incoming connection
	 * or by explicitly connecting to a server.
	 *
	 * The socket provides and input- and an output-stream.
	 * The socket uses non-blocking IO and will e.g. return zero bytes
	 * when nothing is available for reading
	 */
	class Socket {

	private:

		friend class ServerSocket;

		SocketInputStream* is;
		SocketOutputStream* os;

		/**
		 * ctor called from the ServerSocket.
		 * called when the server accepted an incoming connection
		 */
		inline Socket(const int handle);

	public:

		/**
		 * ctor to create an "empty" socket.
		 * use connect() to connect to a server
		 */
		inline Socket();

		/** dtor */
		inline ~Socket();


		/** connect to the given NetworkAddress */
		void connect(const NetworkAddress& target) {

			// ensure the socket is closed
			close();

			// create socket
			handle = socket(AF_INET, SOCK_STREAM, 0);
			if (handle == -1) {throw SocketException("error while creating socket");}

			// connect
			const struct sockaddr_in& addr = target.getAsSocketAddress();
			int ret = ::connect(handle, (struct sockaddr*) &addr, sizeof(struct sockaddr));

			// success?
			if (ret == -1) {
				throw SocketException("error while connecting to: " + target.getHostIP() + ":" + std::to_string(target.getPort()));
			}

			// switch to non-blocking mode
			makeNonBlocking();

		}

		/** close the socket */
		void close() {
			if (handle) {
				::shutdown(handle, SHUT_RDWR);
				::close(handle);
				handle = 0;
			}
		}


	#ifdef WITH_SSL

		/** initiate a SSL handshake to be answered by the other side */
		void startServerSSL() {

			SSL_load_error_strings ();
			SSL_library_init ();

			ssl.handle = nullptr;
			ssl.context = nullptr;
			ssl.enabled = false;

			int ret = 0;

			// create SSLv2/SSLv3/TLSv1 server context
			ssl.context = SSL_CTX_new (SSLv23_server_method ());
			if (!ssl.context) {throw SSLSocketException("error while creating ssl context");}//ERR_print_errors_fp (stderr);}

			std::string privateKey = "";
			std::string cert = "";

			// load the certificate
			ret = SSL_CTX_use_certificate_file(ssl.context, cert.c_str(), SSL_FILETYPE_PEM);
			if (ret == -1) {throw SSLSocketException("Error loading certificate");}

			// lat the private key
			ret = SSL_CTX_use_PrivateKey_file(ssl.context, privateKey.c_str(), SSL_FILETYPE_PEM);
			if (ret == -1) {throw SSLSocketException("Error loading private key");}

			// ensure private key and cert match
			ret = SSL_CTX_check_private_key(ssl.context);
			if (!ret) {throw SSLSocketException("Private key doesn't match certificate file");}

			// create a SSL handle (to write to / read from)
			ssl.handle = SSL_new (ssl.context);
			if (!ssl.handle) {throw SSLSocketException("error while creating ssl handle");}

			// the ssl handle will write to the underlying socket
			ret = SSL_set_fd (ssl.handle, handle);
			if (!ret) {throw SSLSocketException("error while binding ssl socket");}

			// Initiate SSL handshake
			ret = SSL_accept (ssl.handle);
			if (ret != 1) {throw SSLSocketException(std::string("error while starting ssl handshake: "));}

			ssl.enabled = true;

		}

		/** answer a SSL handshake initiated by the other side */
		void startClientSSL() {

			SSL_load_error_strings ();
			SSL_library_init ();

			ssl.handle = nullptr;
			ssl.context = nullptr;
			ssl.enabled = false;

			int ret = 0;

			// create SSLv2/SSLv3/TLSv1 client context
			ssl.context = SSL_CTX_new (SSLv23_client_method ());
			if (!ssl.context) {throw SSLSocketException("error while creating ssl context");}

			// create a SSL handle (to write to / read from)
			ssl.handle = SSL_new (ssl.context);
			if (!ssl.handle) {throw SSLSocketException("error while creating ssl handle");}

			// the ssl handle will write to the underlying socket
			ret = SSL_set_fd (ssl.handle, handle);
			if (!ret) {throw SSLSocketException("error while binding ssl socket");}

			// start SSL handshake
			ret = SSL_connect (ssl.handle);
			if (ret != 1) {throw SSLSocketException(std::string("error while starting ssl handshake: "));}

			// ssl is now enabled
			ssl.enabled = true;

		}

	#endif


		/** get an InputStream to read from this socket */
		inline SocketInputStream* getInputStream();

		/** get an OutputStream to write to this socket */
		inline SocketOutputStream* getOutputStream();


	protected:

		/** the read / write methods can be accessed using streams */

		friend class SocketInputStream;
		friend class SocketOutputStream;

		/** write the given bytes */
		void write(const uint8_t* data, const size_t len) {

			ssize_t ret = 0;

		#ifdef WITH_SSL
			if (ssl.enabled) {
				ret = SSL_write(ssl.handle, data, (int) len);
			} else {
				ret = ::send(handle, data, (size_t) len, MSG_NOSIGNAL);
			}
		#else
			ret = ::send(handle, data, (size_t) len, MSG_NOSIGNAL);
		#endif


			if (ret < 0) {
				throw SocketException("error while writing to socket");
			}

		}

		/** read the given number of bytes */
		ssize_t read(uint8_t* data, const size_t len) {

			//if (!handle) {return -1;}
			ssize_t ret = 0;

		#ifdef WITH_SSL
			if (ssl.enabled) {
				ret = SSL_read(ssl.handle, data, (int) len);
			} else {
				ret = ::recv(handle, data, (size_t) len, 0);//MSG_NOSIGNAL);
			}
		#else
			ret = ::recv(handle, data, (size_t) len, 0);//MSG_NOSIGNAL);
		#endif

			if (ret < 0) {
				if		(errno == EAGAIN)		{return 0;}
				else if (errno == EWOULDBLOCK)	{return 0;}
				else							{return -1;}		// throw SocketException("error while reading from socket");}
			} else if (ret == 0) {
				return -1;						// shutdown by the remote
			}

			return (int) ret;

		}


	private:

		/** switch the socket to non-blocking mode */
		void makeNonBlocking() {
			if (fcntl(handle, F_SETFL, fcntl(handle, F_GETFL) | O_NONBLOCK) < 0) {
				throw SocketException("error while switching to non-blocking mode");
			}
		}



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



#include "SocketInputStream.h"
#include "SocketOutputStream.h"

K::Socket::Socket(const int handle) : handle(handle) {

	// switch to non-blocking mode
	makeNonBlocking();

	is = new SocketInputStream(this);
	os = new SocketOutputStream(this);

}

K::Socket::Socket() : handle(0) {
	is = new SocketInputStream(this);
	os = new SocketOutputStream(this);
}

K::Socket::~Socket() {
	close();
	delete is; is = nullptr;
	delete os; os = nullptr;
}

/** get an InputStream to read from this socket */
K::SocketInputStream* K::Socket::getInputStream() {
	//return K::SocketInputStream(*this);
	return is;
}

/** get an OutputStream to write to this socket */
K::SocketOutputStream* K::Socket::getOutputStream() {
	//return K::SocketOutputStream(*this);
	return os;
}

#endif /* K_SOCKETS_SOCKET_H_ */
