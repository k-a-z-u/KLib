#ifndef K_STREAMS_DICT_DICTHELPER_H
#define K_STREAMS_DICT_DICTHELPER_H

#include "../../streams/InputStream.h"
#include "../../streams/OutputStream.h"

namespace K {

	/**
	 * some helper methods for dictionary streams.
	 * mainly to perform test-cases
	 */
	class DictHelper {

	public:

		static constexpr float dictSize = 1000000;


		/** write a variable-length-code integer */
		static void writeVarLength(OutputStream* os, uint32_t val) {

			if (val <= 0x7F) {
				os->write((val >>  0) & 127);
			} else if (val <= 0x3FFF) {
				os->write((val >>  7) | 128);
				os->write((val >>  0) & 127);
			} else if (val <= 0x1FFFFF) {
				os->write((val >> 14) | 128);
				os->write((val >>  7) | 128);
				os->write((val >>  0) & 127);
			}

		}

		/** read a variable-length-coded integer. returns -1 on EOF */
		static int32_t readVarLength(InputStream* is) {

			uint32_t idx = 0;

			// read variable coded length
			while(true) {
				idx <<= 7;
				int32_t i = is->read();
				if (i == -1) {return -1;}
				idx |= (i & 127);
				if (i < 128) {break;}
			}

			return idx;

		}

	};

}

#endif // K_STREAMS_DICT_DICTHELPER_H
