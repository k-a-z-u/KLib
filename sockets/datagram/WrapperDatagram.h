#ifndef K_SOCKETS_DATAGRAM_WRAPPERDATAGRAM_H
#define K_SOCKETS_DATAGRAM_WRAPPERDATAGRAM_H

#include "Datagram.h"

namespace K {

	/**
	 * @brief this is a convenience class to wrap a datagram around
	 * a pointer-based buffer to make it sendable.
	 *
	 * this datagram can NOT be used for inbound datagrams!
	 *
	 * the provided buffer is NOT copied!
	 * ensure it lives until the datagram is sent!
	 */
	class WrapperDatagram : public Datagram {

	public:

//		/**
//		 * @brief wrap datagram around the given data. the data is NOT copied!!
//		 * @param data the data buffer to wrap
//		 * @param length the lenght of the buffer to wrap
//		 */
//		WrapperDatagram(uint8_t* data, uint32_t length) : data(data), length(length) {
//			;
//		}

		/**
		 * @brief wrap datagram around the given data. the data is NOT copied!!
		 * @param data the data buffer to wrap
		 * @param length the lenght of the buffer to wrap
		 * @param address the destination address for this datagram
		 */
		WrapperDatagram(uint8_t* data, uint32_t length, const NetworkAddress& address) :
			data(data), length(length), address(address) {
			;
		}

		const uint8_t* getData() const override {
			return data;
		}

		uint8_t* getDataWriteable() override {
			throw DatagramException("WrapperDatagram must not be used for inbound datagrams");
		}

		uint32_t getLength() const override {
			return length;
		}

		void ensureSpace(uint32_t space) override {
			(void) space;
			throw DatagramException("WrapperDatagram must not be used for inbound datagrams");
		}

		void setLength(uint32_t length) override {
			(void) length;
			throw DatagramException("WrapperDatagram must not be used for inbound datagrams");
		}

		void setAddress(const NetworkAddress& address) override {
			this->address = address;
		}

		const NetworkAddress& getAddress() const override {
			return this->address;
		}

	private:

		/** the data to send */
		uint8_t* data;

		/** the length of the buffer to send */
		uint32_t length;

		/** the address to send the datagram to */
		NetworkAddress address;

	};

}

#endif // K_SOCKETS_DATAGRAM_WRAPPERDATAGRAM_H
