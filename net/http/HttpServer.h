#ifndef K_NET_HTTP_HTTPSERVER_H
#define K_NET_HTTP_HTTPSERVER_H

#include "../../sockets/Socket.h"
#include "../../sockets/ServerSocket.h"

#include "HttpServerListener.h"
#include "HttpServerRequestHandler.h"

#include "../../log/Logger.h"

#include <thread>
#include <list>
#include <mutex>

namespace K {

	/**
	 * this class contains a very basic HTTP Server waiting
	 * for incoming requests on a defined port
	 */
	class HttpServer {

	private:

		/** the port we are listening on */
		SckPort port;

		/** the socket to listen for incoming connections */
		ServerSocket srvSck;

		/** the reading thread */
		std::thread thread;

		/** listen for connections? */
		HttpServerListener* listener;

		/** used for debug and request logging */
		Logger* log;
		static constexpr const char* logName = "HTTPs";

		/** keep track of all handling threads */
		std::list<HttpServerRequestHandler*> threads;
		std::mutex mtx;

	public:

		/** ctor. given the port to work on */
		HttpServer(const SckPort port) : port(port), listener(nullptr), log(nullptr) {
			;
		}

		/** dtor. will ensure the server is stopped */
		~HttpServer() {
			stop();
		}

		/** no copy */
		HttpServer(const HttpServer& o) = delete;


		/** start the HTTP server and listen for incoming requests */
		void start() {
			if (log) {log->add(logName, LogLevel::INFO, "starting HTTP server on port " + std::to_string(port));}
			srvSck.bind(port);
			thread = std::thread(&HttpServer::accept, this);
		}

		/** stop the HTTP server */
		void stop() {

			// stop the thread handling incoming connections
			if (port) {
				if (log) {log->add(logName, LogLevel::INFO, "stopping HTTP server on port " + std::to_string(port));}
				port = 0;
				srvSck.close();
				if (thread.joinable()) {thread.join();}
			}

			// stop all currently open connections
			stopAllAndJoin();

		}

		/** set the logger to use (if any) */
		void setLogger(Logger* log) {
			this->log = log;
		}

		/** set the listener to call for every request */
		void setListener(HttpServerListener* listener) {
			this->listener = listener;
		}

	private:

		/** stop all currently running handler threads and "join" them */
		void stopAllAndJoin() {

			// call cancel() on each handler
			mtx.lock();
			for (HttpServerRequestHandler* hsrh : threads) { hsrh->cancel(); }
			mtx.unlock();

			// wait for all handlers to terminate (see: done())
			while (!threads.empty()) {usleep(1000*10);}

		}

		/** called form a handler when its handling-loop is complete */
		void done(HttpServerRequestHandler* hsrh) {

			// handler is done -> delete it and remove it from the list of all handlers
			mtx.lock();
			threads.remove(hsrh);
			delete hsrh;
			mtx.unlock();

		}

		/**
		 * accept incoming connections on the server socket
		 * runs within a thread
		 */
		void accept() {

			while (port != 0) {

				try {

					// wait for the next incoming connection
					if (log) {log->add(logName, LogLevel::DEBUG, "waiting for an incoming connection");}
					Socket* sck = srvSck.accept();

					// handle the new HTTP connection
					handleConnection(sck);

				} catch (...) {
					if (port == 0) {return;}					// exception due to server shutdown?
					throw;
				}

			}

			std::cout << "thread done" << std::endl;

		}

		/**
		 * handle an incoming connection:
		 * - parse and respond to all HTTP requests
		 * this loop runs within its own thread and
		 * calls done() as soon as it finishes
		 */
		void handleConnection(Socket* sck) {

			// the handling loop, running in a thread
			auto loop = [this] (HttpServerRequestHandler* hsrh) {
				if (log) {log->add(logName, LogLevel::INFO, "handling incoming connection");}
				hsrh->handle();
				if (log) {log->add(logName, LogLevel::INFO, "connection handled -> closing");}
				done(hsrh);
			};

			// create a new handler and enqueue it (for later cleanup
			HttpServerRequestHandler* hsrh = new HttpServerRequestHandler(sck, listener, log);
			mtx.lock();
			threads.push_back(hsrh);
			mtx.unlock();

			// start the handler in a (detached) background thread
			std::thread thread(loop, hsrh);
			thread.detach();

		}

	};

}

#endif // K_NET_HTTP_HTTPSERVER_H
