#ifndef K_NET_HTTP_HTTPSERVER_H
#define K_NET_HTTP_HTTPSERVER_H

#include "../../sockets/Socket.h"
#include "../../sockets/ServerSocket.h"

#include "HttpServerListener.h"
#include "HttpServerRequestHandler.h"


#include <thread>


namespace K {

	/**
	 * this class contains a very basic HTTP Server waiting
	 * for incoming requests on a defined port
	 */
	class HttpServer {

	public:

		/** ctor. given the port to work on */
		HttpServer(const int port) : port(port) {
			;
		}

		/** start the HTTP server and listen for incoming requests */
		void start() {
			srvSck.bind(port);
			thread = std::thread(&HttpServer::accept, this);
		}

		/** stop the HTTP server */
		void stop() {
			port = 0;
			srvSck.close();
			thread.join();
			std::cout << "thread joined" << std::endl;
		}

		/** set the listener to call for every request */
		void setListener(HttpServerListener* listener) {
			this->listener = listener;
		}

	private:

		/** accept incoming connections within a thread */
		void accept() {

			std::cout << "starting" << std::endl;

			while (port != 0) {

				try {

					std::cout << "waiting to accept" << std::endl;
					Socket* sck = srvSck.accept();
					std::cout << "ACCEPTED!" << std::endl;

					// create a handling thread and detach it from the std::thread
					// the thread will continue to run in the background and the
					// wrapping std::thread will be removed from the stack at the
					// end of the try{}
					std::thread tHandle(&HttpServer::handleConnection, this, sck);
					tHandle.detach();

				} catch (...) {
					if (port == 0) {return;}		// exception due to server shutdown?
					throw;
				}

			}

			std::cout << "thread done" << std::endl;

		}

		/** handle an incoming connection */
		void handleConnection(Socket* sck) {
			HttpServerRequestHandler h(sck, listener);
			h.handle();
		}


	private:

		/** the port we are listening on */
		int port;

		/** the socket to listen for incoming connections */
		ServerSocket srvSck;

		/** the reading thread */
		std::thread thread;

		/** listen for connections? */
		HttpServerListener* listener;

	};

}

#endif // K_NET_HTTP_HTTPSERVER_H
