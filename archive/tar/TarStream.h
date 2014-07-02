#ifndef K_ARCHIVE_TAR_TARSTREAM_H
#define K_ARCHIVE_TAR_TARSTREAM_H

#include "TarEntryHeader.h"
#include "TarHelper.h"
#include "TarStreamEntry.h"
#include "../../streams/OutputStream.h"

namespace K {

	/** one entry within a tar stream */
	struct TarEntry {
		TarEntryHeader header;
		TarStreamEntry& stream;
		TarEntry(const TarEntryHeader& header, TarStreamEntry& stream) : header(header), stream(stream) {;}
	};

	/**
	 * a very very simple TAR file creator
	 * that is able to work on streaming data.
	 */
	class TarStream {


	private:

		/** the stream we are writing to */
		OutputStream* os;

		/** the entry we are currently writing */
		TarEntryHeader curHeader;

		/** the stream we are currently writing to (if any) */
		TarStreamEntry curStream;


	public:

		/** ctor */
		TarStream(OutputStream* os) : os(os) {

			// ensure the current entry is EMPTY
			curHeader.zero();
			curHeader.setSize(0);

		}

		/** dtor */
		~TarStream() {
			close();
		}

		/** add a new file to the archive, identified by header */
		TarEntry addFile(TarEntryHeader& header) {

			// ensure the last file (=current) is closed properly
			curStream.close();

			// calculate the header's checksum
			header.updateChecksum();
			curHeader = header;

			// write the header with its padding
			os->write( (uint8_t*) &header, sizeof(header));
			unsigned int padding = TarHelper::getPadding(sizeof(header), TarHelper::BLOCKSIZE);
			os->write(TarHelper::ZERO_PADDING, padding);

			// create a new writing stream
			curStream = TarStreamEntry(os, curHeader.getSize());
			return TarEntry(curHeader, curStream);

		}

//		/** close the currently active file */
//		void closeCurrent() {

//			// add the file's padding
//			unsigned int padding = TarHelper::getPadding(curEntry.getSize(), TarHelper::BLOCKSIZE);
//			os->write(TarHelper::ZERO_PADDING, padding);
//			std::cout << "write padding: " << padding << std::endl;

//			// subsequent calls to closeCurrent() must not add the padding again
//			curEntry.setSize(0);

//		}

		/** close the archive */
		void close() {

			// ensure we close only once
			if (os == nullptr) {return;}

			// ensure the last file (=current) is closed properly
			curStream.close();

			// add final padding
			os->write(TarHelper::ZERO_PADDING, 2 * TarHelper::BLOCKSIZE);
			os->flush();

			// ensure we close only once
			os = nullptr;

		}



	};


}

#endif // K_ARCHIVE_TAR_TARSTREAM_H
