#ifndef K_ARCHIVE_TAR_TARSTREAMENTRY_H
#define K_ARCHIVE_TAR_TARSTREAMENTRY_H

#include "../../streams/OutputStream.h"
#include "../../streams/StreamException.h"
#include "TarHelper.h"

namespace K {

	/**
	 * OutputStream to write file-contents to a .tar archive.
	 *
	 * the stream prevents writing more bytes to the stream then
	 * previously specified within the corresponding header.
	 */
	class TarStreamEntry : public OutputStream {

	private:

		/** the underlying OutputStream to write the entry to */
		OutputStream* os;

		/** the total size of the entry */
		uint32_t size;

		/** the number of remaining bytes to write */
		uint32_t remaining;


	private:

		friend class TarStream;

		/** hidden ctor */
		TarStreamEntry() : os(nullptr), size(0xFFFFFFFF), remaining(0) {;}

		/** hidden ctor */
		TarStreamEntry(OutputStream* os, const uint32_t size) : os(os), size(size), remaining(size) {;}


	public:

		virtual void write(uint8_t data) override {
			if (remaining == 0) {throw StreamException("can not write more bytes than specified in the header");}
			--remaining;
			os->write(data);
		}

		virtual void write(const uint8_t* data, unsigned int len) override {
			std::cout << remaining << std::endl;
			if (len > remaining) {throw StreamException("can not write more bytes than specified in the header");}
			remaining -= len;
			os->write(data, len);
		}

		virtual void flush() override {
			os->flush();
		}

		virtual void close() override {

			// ensure we close only once!
			if (size == 0xFFFFFFFF) {return;}

			// write the padding bytes after the file
			uint32_t padding = TarHelper::getPadding(size, TarHelper::BLOCKSIZE);
			os->write(TarHelper::ZERO_PADDING, padding);

			// ensure we close only once!
			size = 0xFFFFFFFF;

		}

	};



}

#endif // K_ARCHIVE_TAR_TARSTREAMENTRY_H
