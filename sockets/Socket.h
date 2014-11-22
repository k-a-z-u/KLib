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

//http://svn.netlabs.org/repos/ports/hermes/trunk/src/Socket.cpp
//	private key:		openssl genrsa -out privkey.pem 2048
//	signing-request:	openssl req -new -key privkey.pem -out server.csr
//	certificate:		openssl x509 -req -days 365 -in server.csr -signkey privkey.pem -out server.crt


namespace K {

	class ServerSocket;
	class SocketInputStream;
	class SocketOutputStream;

	/**
	 * socket for network IO
	 */
	class Socket {

	public:

		/** ctor */
		Socket() : handle(0) {
			;
		}

		/** dtor */
		~Socket() {
			close();
		}


		/** connect to the given NetworkAddress */
		void connect(const NetworkAddress& target) {

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

		}

		/** close the socket */
		void close() {
			if (handle) {::close(handle); handle = 0;}
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
		inline SocketInputStream getInputStream();

		/** get an OutputStream to write to this socket */
		inline SocketOutputStream getOutputStream();


	protected:

		/** the read / write methods can be accessed using streams */

		friend class SocketInputStream;
		friend class SocketOutputStream;

		/** write the given bytes */
		void write(const uint8_t* data, unsigned int len) {

			ssize_t ret = 0;

		#ifdef WITH_SSL
			if (ssl.enabled) {
				ret = SSL_write(ssl.handle, data, len);
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
		int read(uint8_t* data, unsigned int len) {

			//if (!handle) {return -1;}
			ssize_t ret = 0;

		#ifdef WITH_SSL
			if (ssl.enabled) {
				ret = SSL_read(ssl.handle, data, len);
			} else {
				ret = ::recv(handle, data, (size_t) len, MSG_NOSIGNAL);
			}
		#else
			ret = ::recv(handle, data, (size_t) len, MSG_NOSIGNAL);
		#endif

			if (ret < 0) {throw SocketException("error while writing reading from socket");}
			return (int) ret;

		}


	private:

		friend class ServerSocket;

		/** create socket from existing handle */
		Socket(int handle) : handle(handle) {
			;
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


/** get an InputStream to read from this socket */
K::SocketInputStream K::Socket::getInputStream() {
	return K::SocketInputStream(*this);
}

/** get an OutputStream to write to this socket */
K::SocketOutputStream K::Socket::getOutputStream() {
	return K::SocketOutputStream(*this);
}


#endif /* K_SOCKETS_SOCKET_H_ */
