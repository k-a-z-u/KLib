#ifndef K_NETWORK_PAYLOAD_H
#define K_NETWORK_PAYLOAD_H

namespace K {

	/** just describes payload: data with length */
	struct Payload {

		/** the data of this payload */
		const uint8_t* data;

		/** the length of this payload */
		const uint32_t length;

		/** ctor */
		Payload(const uint8_t* data, const uint32_t length) : data(data), length(length) {;}

		/** is this an empty payload? */
		bool isEmpty() const {return length == 0;}

	public:

		/** get an empty payload */
		static Payload empty() {return Payload();}		

	private:

		/** empty payload */
		Payload() : data(nullptr), length(0) {;}

	};


}

#endif // K_NETWORK_PAYLOAD_H
