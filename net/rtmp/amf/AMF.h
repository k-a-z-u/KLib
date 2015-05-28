#ifndef K_NET_RTMP_AMF_AMF_H
#define K_NET_RTMP_AMF_AMF_H

#include <cstdint>
#include "AMFEntries.h"

namespace K {

	class AMF {

	public:

		static AMFResult parse(const uint8_t* data, const uint32_t len) {
			const int start = data[0] == 0x00 ? 1 : 0;	// WHY? perhaps difference between AMF0 and AMF3?
			AMFResult res;
			for (int i = start; i < len; ) {
				res.entries.push_back( get(&data[i], i) );
			}
			return res;
		}

		static AMFEntryObject* getObject(const uint8_t* data, int& pos) {

			AMFEntryObject* obj = new AMFEntryObject();
			uint32_t code;
			int offset = 0;
			do {
				std::string key = getString(&data[offset], offset);
				AMFEntry* val = get(&data[offset], offset);
				obj->entries.push_back(AMFEntryKeyVal(key, val));
				code = (data[offset+0] << 16) | (data[offset+1] << 8) | (data[offset+2] << 0);
			} while (code != 0x000009);
			pos += offset;
			return obj;

		}

		static AMFEntry* get(const uint8_t* data, int& pos) {
			++pos;
			switch (data[0]) {
				case 0x00:	return new AMFEntryNumber(getNumber(&data[1], pos));
				case 0x02:	return new AMFEntryString(getString(&data[1], pos));
				case 0x03:	return getObject(&data[1], pos);
				case 0x05:	return new AMFEntryNull();
				default:	pos += 100000; return nullptr;
			}
		}

	private:

		/** get a string, written at data. return the string and increment the position */
		static std::string getString(const uint8_t* data, int& pos) {
			//std::cout << "string" << std::endl;
			const uint16_t len = (data[0] << 8) | (data[1] << 0);
			const std::string str((const char*) &data[2], len);
			pos += 2+len;
			return str;
		}

		/** get a number, written at data. return the number and increment the position */
		static double getNumber(const uint8_t* data, int& pos) {
			pos += 8;
			return *((double*)&data[0]);
		}


	};

}

#endif // K_NET_RTMP_AMF_AMF_H
