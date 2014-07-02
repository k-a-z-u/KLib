#ifndef K_ARCHIVE_TAR_TARHELPER_H
#define K_ARCHIVE_TAR_TARHELPER_H

namespace K {

	/**
	 * contains some helper methods, needed for tar archives
	 */
	class TarHelper {


	public:

		/** the blocksize to pad everything to */
		static constexpr int BLOCKSIZE = 512;

		/** pre-initialized zero padding */
		static constexpr uint8_t ZERO_PADDING[BLOCKSIZE*2] = {0};


	public:

		/** round up the the nearest multiple of "multiple" */
		static unsigned int roundUp(const unsigned int value, const unsigned int multiple) {
			return value + multiple - 1 - (value - 1) % multiple;
		}

		/** get the necessary padding when storing "value" while using "blocksize" */
		static unsigned int getPadding(const unsigned int value, const unsigned int blocksize) {
			return roundUp(value, blocksize) - value;
		}

		/** convert ordinal ascii value to an interger */
		static uint32_t ordAsciiToInt(const char* ptr, int num) {
			uint64_t val = 0;
			do {
				val = (val << 3) | (*ptr - '0');
				++ptr;
			} while(--num != 1);
			return val;
		}

		/** convert the given integer as ordinal ascii into dst */
		static void intToOrdAscii(uint32_t val, char* dst, unsigned int len) {
			--len;
			dst += len;
			*dst = 0;
			--dst;
			do {
				*dst = (char) ( (val & 7) + '0' );
				--dst;
				val >>= 3;
			} while(--len);
		}

	};

	// static variables
	constexpr uint8_t TarHelper::ZERO_PADDING[];

}

#endif // K_ARCHIVE_TAR_TARHELPER_H
