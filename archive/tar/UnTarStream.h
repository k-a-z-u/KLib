#ifndef K_ARCHIVE_TAR_UNTARSTREAM_H
#define K_ARCHIVE_TAR_UNTARSTREAM_H

#include "../../streams/InputStream.h"
#include "../../streams/StreamException.h"

#include "TarConstants.h"
#include "TarEntryHeader.h"
#include "UnTarStreamEntry.h"

#include <vector>

namespace K {

	struct UnTarEntry {
		TarEntryHeader header;
		UnTarStreamEntry& stream;
		UnTarEntry(const TarEntryHeader& header, UnTarStreamEntry& stream) : header(header), stream(stream) {;}
	};

	/**
	 * a very very simple TAR file extractor (un-tar)
	 * that is able to work on streaming data.
	 */
	class UnTarStream {


	private:

		/** the underlying input stream */
		InputStream* is;

		/** the header of the tar entry we are currently working on */
		TarEntryHeader curHeader;

		/** the stream for the current file-entry (if any) */
		UnTarStreamEntry curStream;

		/** is the current entry valid, or do we need to load a new chunk? */
		bool chunkIsLoaded;

		/** found eof or stream corrupted? */
		bool eof;






	public:

		/** ctor */
		UnTarStream(InputStream* is) : is(is), chunkIsLoaded(false), eof(false) {

			// set the current header to empty
			curHeader.zero();
			curHeader.setSize(0);

		}

		/** are there more files within the tar? */
		bool hasNext() {

			// do we need to load a new chunk?
			// (this just prevents from multiple calls to hasNext())
			if (!chunkIsLoaded) {moveToNextFile();}

			// is the current entry a file?
			return !isEOF();

		}

		/** get the next file within the tar */
		UnTarEntry next() {

			// if nobody called hasNext(), we do so here
			if (!chunkIsLoaded) {moveToNextFile();}

			// we need to load a new entry for the next hasNext() call
			chunkIsLoaded = false;

			// set the new input stream for the current entry
			curStream = UnTarStreamEntry(is, curHeader.getSize());
			return UnTarEntry(curHeader, curStream);

		}


	private:



		// is the current entry an empty chunk? -> EOF
		bool isEOF() {
			return eof || strlen(curHeader.name) == 0;
		}

		/** advance within the TAR until a file is found */
		void moveToNextFile() {

			// ensure the InputStream is correctly positioned
			curStream.close();

			// skip non-file entries until a NORMAL_FILE entry is found
			while (true) {

				// read the next tar header
				readNextHeader();

				// EOF reached?
				if (isEOF()) {break;}

				// found a file?
				if (curHeader.typeflag == TAR_TYPE_NORMAL_FILE) {break;}

				// not a file. skip the payload (if any)
				skipEntry();

			}

			// we loaded the next chunk, ready to process (even if its EOF)
			chunkIsLoaded = true;

		}

		/** just skip the current entry and move to the next one */
		void skipEntry() {
			const unsigned int paddedSize = TarHelper::roundUp(curHeader.getSize(), TarHelper::BLOCKSIZE);
			is->skip(paddedSize);
		}

		/** read the next 512 byte header */
		void readNextHeader() {

			// read the next header
			int read = is->readFully( (uint8_t*) &curHeader, sizeof(curHeader) );
			if (read != sizeof(curHeader)) {
				std::cout << "read: " << read << std::endl;
				throw StreamException("failed to read a complete header chunk");
			}

			// skip the padding bytes (to fill BLOCKSIZE)
			unsigned int padding = TarHelper::getPadding(sizeof(TarEntryHeader), TarHelper::BLOCKSIZE);
			is->skip(padding);

			std::cout << "read: " << read << std::endl;
			std::cout << "padding: " << padding << std::endl;

			// check for an empty chunk -> EOF
			static const char EMPTY_MAGIC[6] = {0};
			if ( memcmp( EMPTY_MAGIC, curHeader.magic, 6 ) == 0 ) {
				eof = true;
				return;
			}

			// ensure we found a correct header
			if ( memcmp( "ustar", curHeader.magic, 6 ) != 0 ) {
				throw StreamException("invalid tar-header found. file corrupted?");
			}

			// ensure the header's checksum is correct
			if (curHeader.getCheckSum() != curHeader.calcCheckSum()) {
				throw StreamException("header's checksum is not correct!");
			}

		}

	};

}

#endif // K_ARCHIVE_TAR_UNTARSTREAM_H
