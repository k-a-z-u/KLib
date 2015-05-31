#ifndef K_NET_RTMP_AMF_AMF_H
#define K_NET_RTMP_AMF_AMF_H

#include <cstdint>
#include <exception>

#include "AMFEntries.h"

namespace K {

	class AMFException : public std::exception {
	private:
		std::string str;
	public:
		AMFException(const std::string& str) : str(str) {;}
		const char* what() const throw() {return str.c_str();}
	};

	class AMF {

	private:

		const uint8_t* data;
		const uint32_t len;

		// current position within data
		uint32_t idx;

	public:

		/** ctor */
		AMF(const uint8_t* data, const uint32_t len) : data(data), len(len), idx(0) {

		}

	public:

		/** parse the AMF data and silently ignore errors */
		AMFResult parseErroneousData(bool& gotErrors) {

			// sometimes the first byte is a padding zero.. could not find an explanation yet.. others had the same question
			// https://forums.adobe.com/thread/651219?tstart=0
			idx = data[0] == 0x00 ? 1 : 0;

			gotErrors = false;

			AMFResult res;
			try {
				while (idx < len) {
					res.entries.push_back( consume() );
				}
			} catch (AMFException& e) {
				gotErrors = true;
			}

			return res;

		}

		/** parse an AMF-Object */
		AMFEntryObject* getObject() {

			AMFEntryObject* obj = new AMFEntryObject();
			uint32_t code;

			do {

				// get the entry's key and value and append the newly created object
				const std::string key = getString();
				AMFEntry* val = consume();
				if (val) {obj->entries.push_back(AMFEntryKeyVal(key, val));}

				// end-of-object marker reached?
				code = readInt24();

			} while (code != 0x000009);

			// jump at the end of all objects + the 3 byte end-of-objects marker
			idx += 3;
			return obj;

		}

		/** parse an AMF-ECMA-Array */
		AMFEcmaArray* getEcmaArray() {

			AMFEcmaArray* obj = new AMFEcmaArray();

			const int arrLen = consumeInt32(data);

			while(1) {

				// end-of-object reached?
				const int code = readInt24();
				if (code == 0x000009) {break;}

				// get the entry's name and value
				const std::string& key = getString();
				AMFEntry* val = consume();
				if (val) {obj->entries.push_back(AMFEntryKeyVal(key, val));}

			}

			// jump at the end of all objects + the 3 byte end-of-objects marker
			idx += 3;
			return obj;

		}

		/** get a 24-bit int. used to search for the end-of-object marker */
		int readInt24() {
			return (data[idx+0] << 16) | (data[idx+1] << 8) | (data[idx+2] << 0);
		}

		/** read (and consume) a 32-bit integer */
		int consumeInt32(const uint8_t* data) {
			uint32_t val = (data[idx+0] << 24) | (data[idx+1] << 16) | (data[idx+2] << 8) | (data[idx+3] << 0);
			idx += 4;
			return val;
		}

		/** read (and consume) a 16-bit integer */
		int consumeInt16() {
			uint16_t val = (data[idx+0] << 8) | (data[idx+1] << 0);
			idx += 2;
			return val;
		}

		AMFEntry* consume() {

			// sanity check
			if (idx > len) {throw AMFException("reading beyond data size. something went wrong");}

			const uint8_t type = data[idx]; ++idx;
			switch (type) {
				case 0x00:	return new AMFEntryNumber(getNumber());
				case 0x02:	return new AMFEntryString(getString());
				case 0x03:	return getObject();
				case 0x05:	return new AMFEntryNull();
				case 0x08:	return getEcmaArray();
				default:	throw AMFException("unsupported element type: " + std::to_string(type));
			}
		}

	private:

		/** get a string, written at data. return the string and increment the position */
		std::string getString() {
			const uint16_t slen = consumeInt16();
			if (slen+idx > len) {throw AMFException("error. string beyond data size");}
			const std::string str((const char*) &data[idx], slen);
			idx += slen;
			return str;
		}

		/** get a number, written at data. return the number and increment the position */
		double getNumber() {
			const double d = *((double*)&data[idx]);
			idx += 8;
			return d;
		}


	};

}

#endif // K_NET_RTMP_AMF_AMF_H
