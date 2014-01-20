/*
 * Archive7z.h
 *
 *  Created on: Dec 28, 2013
 *      Author: kazu
 */

#ifndef ARCHIVE7Z_H_
#define ARCHIVE7Z_H_

extern "C" {
#include "../inc/7z/7z.h"
#include "../inc/7z/7zAlloc.h"
#include "../inc/7z/7zFile.h"
#include "../inc/7z/7zCrc.h"
#include "../inc/7z/7zVersion.h"
}

#include <vector>
#include "../string/String.h"
#include "../fs/File.h"
#include "../Exception.h"

#include <algorithm>



namespace K {

typedef std::function< void(const std::string& file, float percent)> Archive7zCallback;

	/**
 * exceptions for the 7z archiver
 */
	class Archive7zException : public Exception {
	public:
		Archive7zException(const std::string& msg) : Exception(msg) {;}
	};


	class Archive7z;


	/**
 * composite data-structure for 7z
 * to keep folders and contained files with a simple
 * decompression method
 */
	class Archive7zComposite {

	public:

		/** is this a folder (or a file)? */
		bool isFolder() const {return folder;}

		/** get all child elements (if this is a folder) */
		const std::vector<Archive7zComposite>& getChilds() const {return childs;}

		/** get the name of this entry */
		const std::string& getName() const {return name;}

		/**
	 * decompress this file to the given output buffer.
	 * only works for files. when trying to decompress a folder,
	 * and exception will be thrown!
	 */
		void decompress(std::vector<uint8_t>& dst) const;

		//	/** print archive structure */
		//	friend std::ostream& operator << (std::ostream& out, Archive7zComposite& me) {
		//		out << ((me.isFolder()) ? ("D: ") : ("F: "));
		//		out << me.name << " (" << me.idx << ")" << std::endl;
		//		for (Archive7zComposite& c : me.childs) {out << c;}
		//		return out;
		//	}


	private:

		/** ctor */
		friend class Archive7z;
		Archive7zComposite(Archive7z& archive, const std::string& name, bool isFolder, unsigned int idx = 0) :
			archive(archive), name(name), folder(isFolder), idx(idx) {;}

		/** add a new subfolder to this composite */
		Archive7zComposite& addFolder(Archive7z& archive, const std::string& name, unsigned int idx = 0) {
			for (Archive7zComposite& c : childs) {
				if (c.isFolder() && c.name == name) {c.idx = idx; return c;}
			}
			childs.push_back(Archive7zComposite(archive, name, true));
			return childs[childs.size()-1];
		}

		/** add a new file to this composite */
		void addFile(Archive7z& archive, const std::string& name, unsigned int idx) {
			childs.push_back(Archive7zComposite(archive, name, false, idx));
		}



	private:

		/** the archive this file belongs to */
		Archive7z& archive;

		/** all child elements within this composite */
		std::vector<Archive7zComposite> childs;

		/** the folder- / file-name */
		std::string name;

		/** is this a folder or a file? */
		bool folder;

		/** the element's index within the 7z-archive */
		unsigned int idx;


	};

	class Archive7z {

	public:

		/**
	 * ctor
	 * @param file open the given 7z archive
	 */
		Archive7z(const File& f) : root(*this, "/", true) {

			SRes res;
			UInt16 temp[4096];

			allocImp.Alloc = SzAlloc;
			allocImp.Free = SzFree;

			allocTempImp.Alloc = SzAllocTemp;
			allocTempImp.Free = SzFreeTemp;

			if (InFile_Open(&archiveStream.file, f.getAbsolutePath().c_str())) {
				throw Archive7zException("can not open input file: " + f.getAbsolutePath());
			}

			FileInStream_CreateVTable(&archiveStream);
			LookToRead_CreateVTable(&lookStream, False);

			lookStream.realStream = &archiveStream.s;
			LookToRead_Init(&lookStream);

			CrcGenerateTable();

			SzArEx_Init(&db);
			res = SzArEx_Open(&db, &lookStream.s, &allocImp, &allocTempImp);
			if (res != SZ_OK) {throw Archive7zException("error while opening archive");}


			// read all files
			std::cout << db.db.NumFiles << std::endl;
			for (unsigned int i = 0; i < db.db.NumFiles; ++i) {

				// file description
				const CSzFileItem* f = db.db.Files + i;

				// get the file-name
				SzArEx_GetFileNameUtf16(&db, i, temp);
				std::string file = toString(temp);
				std::vector<std::string> elems = String::split(file, '/');

				// create folder structure
				Archive7zComposite* node = &root;
				for (unsigned int j = 0; j < elems.size() - 1; ++j) {
					node = &node->addFolder(*this, elems[j]);
				}

				// add last layer (the deepest element)
				if (!f->IsDir) {
					node->addFile(*this, elems[elems.size()-1], i);
				} else {
					// this will only update the folder's idx (database index)
					node->addFolder(*this, elems[elems.size()-1], i);
				}

			}

		}

		/** dtor */
		~Archive7z() {

			// cleanup
			SzArEx_Free(&db, &allocImp);
			File_Close(&archiveStream.file);

		}

		/** get a composite structure for all files and folders within this archive */
		Archive7zComposite getFiles() {
			return root;
		}

		/** this method will walk all composites and extract them into the given path */
		void extractTo(const K::File& folder, Archive7zCallback cb = nullptr) {
			Archive7zComposite comp = getFiles();
			int cnt = 0;
			extractTo(folder, comp, cnt, cb);
		}

	private:

		/** extract all childs of comp to the given folder */
		void extractTo(const K::File& folder, Archive7zComposite& comp, int& cnt, Archive7zCallback cb) {
			for (Archive7zComposite child : comp.getChilds()) {

				if (child.isFolder()) {
					K::File subFolder(folder, child.getName());
					subFolder.mkdir();
					extractTo(subFolder, child, cnt, cb);
					++cnt;
				} else {
					std::vector<uint8_t> buf;
					child.decompress(buf);
					K::File f(folder, child.getName());
					f.write(buf.data(), (unsigned int) buf.size());
					++cnt;
					if (cb) { cb( child.getName(), float(cnt) / float(db.db.NumFiles) ); }
				}

			}
		}

		friend class Archive7zComposite;

		/**
	 * extract the file identified by idx into the given buffer
	 * @param idx the files index within the archive
	 * @param dst the destination to decompress the file to
	 */
		void extract(unsigned int idx, std::vector<uint8_t>& dst) {

			UInt32 blockIndex = 0xFFFFFFFF;		/* it can have any value before first call (if outBuffer = 0) */
			Byte *outBuffer = 0;				/* it must be 0 before first call for each new archive. */
			size_t outBufferSize = 0;
			size_t offset = 0;
			size_t outSizeProcessed = 0;

			// decompress completely
			SRes res = SzArEx_Extract(&db, &lookStream.s, idx,
									  &blockIndex, &outBuffer, &outBufferSize,
									  &offset, &outSizeProcessed,
									  &allocImp, &allocTempImp);

			// check result
			if (res != SZ_OK) {throw Archive7zException("error while decompressing file");}

			// pointer to decompressed data and length
			uint8_t* data = outBuffer + offset;
			size_t len = outSizeProcessed;

			// copy to output
			dst.resize(len);
			memcpy(&dst[0], data, len);

			// cleanup
			IAlloc_Free(&allocImp, outBuffer);

		}

	private:

		/** convert wchar_t to char_t */
		static std::string toString(const UInt16* s) {
			std::string ret = "";
			while (*s) {
				ret += (uint8_t) *s;
				++s;
			}
			return ret;
		}

		/** the root folder containing all files */
		Archive7zComposite root;

		CFileInStream archiveStream;
		ISzAlloc allocImp;
		ISzAlloc allocTempImp;
		CLookToRead lookStream;
		CSzArEx db;

	};


	/** implement forward-declaration of decompress() */
	void Archive7zComposite::decompress(std::vector<uint8_t>& dst) const {
		if (isFolder()) {throw Archive7zException("uncompressing folders is not supported!");}
		archive.extract(idx, dst);
	}


}

#endif /* ARCHIVE7Z_H_ */
