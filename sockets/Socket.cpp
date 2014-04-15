/*
 * Socket.cpp
 *
 *  Created on: Dec 15, 2013
 *      Author: kazu
 */


#include "Socket.h"
#include "SocketException.h"
#include "SSLSocketException.h"


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

Socket::Socket() : handle(0) {
	;
}

Socket::Socket(int handle) : handle(handle) {

}

Socket::~Socket() {
	close();
}

void Socket::connect(const NetworkAddress& target) {

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

#ifdef WITH_SSL

void Socket::startServerSSL() {

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

void Socket::startClientSSL() {

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

SocketInputStream Socket::getInputStream() {
	return SocketInputStream(*this);
}

SocketOutputStream Socket::getOutputStream() {
	return SocketOutputStream(*this);
}


void Socket::write(uint8_t* data, unsigned int len) {
	ssize_t ret = 0;
	if (ssl.enabled) {
#ifdef WITH_SSL
		ret = SSL_write(ssl.handle, data, len);
#else
		throw new SocketException("compiled without SSL support");
#endif
	} else {
		//ret = ::write(handle, data, (size_t) len);
		ret = ::send(handle, data, (size_t) len, MSG_NOSIGNAL);
	}
	if (ret < 0) {throw SocketException("error while writing to socket");}
}



int Socket::read(uint8_t* data, unsigned int len) {
	//if (!handle) {return -1;}
	ssize_t ret = 0;
	if (ssl.enabled) {
#ifdef WITH_SSL
		ret = SSL_read(ssl.handle, data, len);
#else
		throw new SocketException("compiled without SSL support");
#endif
	} else {
		//ret = ::read(handle, data, (size_t) len);
		ret = ::recv(handle, data, (size_t) len, MSG_NOSIGNAL);
	}
	if (ret < 0) {throw SocketException("error while writing reading from socket");}
	return (int) ret;
}


void Socket::close() {
	if (handle) {::close(handle); handle = 0;}
}

}
