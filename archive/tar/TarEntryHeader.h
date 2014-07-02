#ifndef K_ARCHIVE_TAR_TARENTRYHEADER_H
#define K_ARCHIVE_TAR_TARENTRYHEADER_H

#include "TarHelper.h"

class TarHeader_checkSum_Test;

namespace K {

	/**
	 * this header struct describes one entry (file)
	 * within a .tar archive.
	 * it can be used for both, reading and writing .tar files.
	 */
	struct TarEntryHeader {

	private:

		friend class UnTarStream;
		friend class TarStream;
		friend class TarHeader_checkSum_Test;

		/** the entry's name (e.g. filename) */
		char name[100];

		char mode[8];

		/** the entry's UID (ordinal ascii) */
		char uid[8];

		/** the entry's GID (ordinal ascii) */
		char gid[8];

		/** the entry's real size in bytes (ordinal ascii) */
		char size[12];

		char mtime[12];
		char chksum[8];
		char typeflag;
		char linkname[100];
		char magic[6];
		char version[2];
		char uname[32];
		char gname[32];
		char devmajor[8];
		char devminor[8];
		char prefix[155];


	public:

		/** get a new, empty header, pre-filled with timestamp, magic-byte, etc.. */
		static TarEntryHeader getEmptyHeader() {

			TarEntryHeader teh;

			memset(&teh, 0, sizeof(TarEntryHeader));
			memcpy(teh.mode, "000664 ", 7);
			memcpy(teh.uid, "001750 ", 7);
			memcpy(teh.gid, "001750 ", 7);
			teh.setSize(0);
			teh.setTimestamp(0);
			//memcpy(teh.mtime, "10000000000 ", 12);

			memcpy(teh.magic, "ustar", 5);
			memcpy(teh.version, "00", 2);
			memcpy(teh.uname, "none", 4);
			memcpy(teh.gname, "none", 4);
			memcpy(teh.devmajor, "000000 ", 7);
			memcpy(teh.devminor, "000000 ", 7);

			return teh;

		}

		/** get a new header for a file of the given size */
		static TarEntryHeader getFileHeader(const std::string& fileName, unsigned int size) {

			TarEntryHeader teh = getEmptyHeader();
			teh.setSize(size);
			teh.setFileName(fileName);
			teh.setType(TAR_TYPE_NORMAL_FILE);

			return teh;

		}

		/** get a new header for a directory */
		static TarEntryHeader getDirHeader(const std::string& dirName) {

			TarEntryHeader teh = getEmptyHeader();
			teh.setSize(0);
			teh.setFileName(dirName);
			teh.setType(TAR_TYPE_DIRECTORY);

			return teh;

		}


	private:

		/** hidden ctor. use factory method(s) instead */
		TarEntryHeader() {

		}

		/** set all bytes to zero */
		void zero() {
			memset(this, 0, sizeof(TarEntryHeader));
		}

		/** calculate the headers's checksum */
		unsigned int calcCheckSum() {
			char tmp[8];
			memcpy(tmp, chksum, 8);
			memcpy(chksum, "        ", 8);
			int sum = 0;
			for (unsigned int i = 0; i < sizeof(TarEntryHeader); ++i) { sum += ((uint8_t*)this)[i]; }
			memcpy(chksum, tmp, 8);
			return sum;
		}


	public:

		/** update the header's checksum depending on its other values */
		void updateChecksum() {
			unsigned int sum = calcCheckSum();
			TarHelper::intToOrdAscii(sum, chksum, 7);
		}



		/** get the currently stored checksum */
		uint32_t getCheckSum() {
			return (uint32_t) TarHelper::ordAsciiToInt(chksum, 7);
		}


		/** get the entry's size in bytes */
		uint32_t getSize() const {return (uint32_t) TarHelper::ordAsciiToInt(size,12);}

		/** set the entry's size in bytes */
		void setSize(const unsigned int s) {TarHelper::intToOrdAscii(s, size, 12); size[11] = 0x20;}

		///** get the entry's size in bytes as multiples of 512 */
		//unsigned int getBlockedSize() const { return TarHelper::roundUp(getSize(), 512); }


		/** get the entry's filename */
		std::string getFileName() const {return std::string(name);}

		/** set the entry's filename */
		void setFileName(const std::string& str) {
			unsigned int strLen = (unsigned int) str.length();
			unsigned int maxLen = (unsigned int) sizeof(name) - 1;
			unsigned int len = (strLen < maxLen) ? (strLen) : (maxLen);
			memcpy(name, str.data(), len);
			name[len] = 0;
		}


		/** get the unix timestamp for this entry */
		uint32_t getTimestamp() const {
			uint32_t ts = TarHelper::ordAsciiToInt(mtime, 12);
			return ts;
		}

		/** set the unix timestamp for this entry */
		void setTimestamp(const uint32_t ts) {
			TarHelper::intToOrdAscii(ts, mtime, 12);
		}

		/** set the entry's timestamp to "now" */
		void setTimestampNow() {
			setTimestamp( (uint32_t) time(nullptr) );
		}



		/** set the type for this entry (file, dir, ...) */
		void setType(uint8_t type) {
			typeflag = type;
		}



	};

}

#endif // TARENTRYHEADER_H
