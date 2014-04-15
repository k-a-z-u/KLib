#ifndef K_SOCKETS_DATAGRAM_DEFAULTDATAGRAM_H
#define K_SOCKETS_DATAGRAM_DEFAULTDATAGRAM_H

#include <vector>
#include "Datagram.h"

namespace K {

	/**
	 * @brief this is the default datagram implementation using
	 * a std::vector to store in/and outbound data.
	 */
	class DefaultDatagram : public Datagram {

	public:

		/** ctor for empty datagram */
		DefaultDatagram() : usedBytes(0) {;}

		/**
		 * @brief ctor that copies the given data into the internal buffer
		 * @param data the data to copy and send
		 * @param length the length of the data to copy and send
		 * @param address the address to send the datagram to
		 */
		DefaultDatagram(uint8_t* data, uint32_t length, const NetworkAddress& address) : address(address) {
			buffer.resize(length);
			memcpy(buffer.data(), data, length);
		}

		const uint8_t* getData() const override {
			return buffer.data();
		}

		uint8_t* getDataWriteable() override {
			return buffer.data();
		}

		uint32_t getLength() const override {
			return usedBytes;
		}

		void ensureSpace(uint32_t space) override {
			buffer.reserve(space);
		}

		void setLength(uint32_t length) override {
			this->usedBytes = length;
		}

		void setAddress(const NetworkAddress& address) override {
			this->address = address;
		}

		const NetworkAddress& getAddress() const override {
			return this->address;
		}

	private:

		/** the internal data buffer */
		std::vector<uint8_t> buffer;

		/** track the number of used bytes in the vector.
		 * this makes resize() obsolete and speeds things up */
		uint32_t usedBytes;

		/** the network address to send the datagram to / it was received from */
		NetworkAddress address;

	};

}

#endif // K_SOCKETS_DATAGRAM_DEFAULTDATAGRAM_H
