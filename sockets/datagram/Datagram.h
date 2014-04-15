#ifndef K_SOCKETS_DATAGRAM_DATAGRAM_H
#define K_SOCKETS_DATAGRAM_DATAGRAM_H

#include <string>

#include "../address/NetworkAddress.h"

namespace K {

	/** exception handling */
	class DatagramException : public std::exception {
	public:
		DatagramException(const std::string& msg) : msg(msg) {;}
		const char* what() const throw() override {return msg.c_str();}
	private:
		std::string msg;
	};

	/**
	 * @brief interface for all datagrams to send / receive via UDP.
	 *
	 * outbound datagrams have a databuffer with a specific size to be sent.
	 *
	 * inbound datagrams will receive a call for the expected datagram size
	 * and must then allocate enough bytes to hold a datagram of the given size.
	 */
	class Datagram {

	public:

		/**
		 * @brief get a pointer to the data to send. This serves also
		 * for the pointer to the buffer to write a newly received datagram to
		 * @return the pointer to the buffer
		 */
		virtual const uint8_t* getData() const = 0;

		/**
		 * @brief get the length of the data in the buffer.
		 * @return
		 */
		virtual uint32_t getLength() const = 0;


		/**
		 * @brief same as getData() but allows writing to the returned buffer.
		 * the buffer is "initialized" using ensureSpace() before this method is accessed.
		 * @return a writeable representation of the internal data
		 */
		virtual uint8_t* getDataWriteable() = 0;

		/**
		 * @brief this method is called before receiving a new datagram and
		 * accessing getData() to get the buffer to write this datagram to.
		 * the implementation must ensure the internal buffer behind getData()
		 * can hold at least the given number of bytes
		 * @param space the number of bytes the underlying buffer must be able to hold
		 */
		virtual void ensureSpace(uint32_t space) = 0;

		/**
		 * @brief a datagram has been received and its length is now known. enform the
		 * datagram about this new length. subsequent calls to getLength() must return the
		 * same length provided here!
		 * @param length the length of valid bytes within the internal buffer
		 */
		virtual void setLength(uint32_t length) = 0;

		/**
		 * @brief set the destination address for this datagram. this is the network
		 * address (e.g. host/port) to send the datagram to.
		 * @param address the address to send the datagram to
		 */
		virtual void setAddress(const NetworkAddress& address) = 0;

		/**
		 * @brief if this datagram is used for outbound purpose, this will return the
		 * network address to send the datagram to.
		 * if this is an inbound datagram, received from a socket, this method will return
		 * the network address of the datagram's sender.
		 * @return
		 */
		virtual const NetworkAddress& getAddress() const = 0;

	};

}

#endif // K_SOCKETS_DATAGRAM_DATAGRAM_H
