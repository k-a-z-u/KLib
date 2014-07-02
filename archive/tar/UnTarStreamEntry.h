#ifndef K_ARCHIVE_TAR_UNTARSTREAMENTRY_H
#define K_ARCHIVE_TAR_UNTARSTREAMENTRY_H

#include "../../streams/InputStream.h"
#include "../../streams/StreamException.h"
#include "TarHelper.h"
#include <vector>

namespace K {

	/**
	 * stream to read one entry (file) of a .tar archive.
	 *
	 * the stream provides exactly the number of bytes as
	 * specified within the corresponding tar header.
	 */
	class UnTarStreamEntry : public InputStream {

	private:

		/** the underlying input stream to read the entry from */
		InputStream* is;

		/** the total size of the entry */
		uint32_t size;

		/** the number of bytes remaining for reading */
		uint32_t remaining;



	public:

		friend class Tar;

		/** ctor */
		UnTarStreamEntry() : is(nullptr), size(0xFFFFFFFF), remaining(0) {;}

		/** ctor */
		UnTarStreamEntry(InputStream* is, const uint32_t size) : is(is), size(size), remaining(size) {;}

	public:


		virtual int read() {

			// anything to read?
			if (remaining == 0) {return -1;}

			// read one byte
			--remaining;
			return is->read();

		}

		virtual int read(uint8_t* data, unsigned int len) override {

			uint32_t max = (len < remaining) ? (len) : (remaining);
			uint32_t numRead = is->read(data, max);
			remaining -= numRead;
			std::cout << "read " << numRead << " bytes. remaining is " << remaining << std::endl;
			return numRead;

		}

		virtual void skip(const uint64_t n) override {

			// check whether there are enough bytes remaining to skip
			uint32_t _n = (uint32_t) n;
			uint32_t max = (_n < remaining) ? (_n) : (remaining);
			remaining -= max;
			is->skip(max);

		}

		virtual void close() override {

			// ensure we close only once
			if (size == 0xFFFFFFFF) {return;}

			// skip all remaining bytes
			is->skip(remaining);
			remaining = 0;

			// skip all zero padding bytes
			uint32_t padding = TarHelper::getPadding(size, TarHelper::BLOCKSIZE);
			is->skip(padding);

			// ensure we close only once
			size = 0xFFFFFFFF;

		}

		/** convenience method to read the whole payload into an std::vector */
		std::vector<uint8_t> readCompletely() {
			std::vector<uint8_t> vec;
			vec.resize(remaining);
			readFully(vec.data(), remaining);
			return vec;
		}


	};

}

#endif // K_ARCHIVE_TAR_UNTARSTREAMENTRY_H
