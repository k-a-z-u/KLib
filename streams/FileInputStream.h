/*
 * FileInputStream.h
 *
 *  Created on: Dec 18, 2013
 *      Author: kazu
 */

#ifndef FILEINPUTSTREAM_H_
#define FILEINPUTSTREAM_H_

#include "InputStream.h"
#include "StreamException.h"
#include "../fs/File.h"

namespace K {

/**
 * Stream to read data from a file
 */
class FileInputStream : public InputStream {

public:

	/** ctor */
	FileInputStream(const std::string& file) : fp(nullptr) {
		open(file);
	}

	FileInputStream(const File& file) : FileInputStream(file.getAbsolutePath()) {
		;
	}

	/** dtor */
	~FileInputStream() {
		close();
	}

	int read() override {
		if (feof(fp)) {return -1;}
		return fgetc(fp);
	}

	int read(uint8_t* data, unsigned int len) override {
		if (feof(fp)) {return -1;}
		unsigned int read = (unsigned int) fread(data, 1, len, fp);
		return read;
	}

	void close() override {
		if (fp) {fclose(fp); fp = nullptr;}
	}

	void skip(const uint64_t n) override {
		int ret = fseek(fp, n, SEEK_CUR);
		if (ret != 0) {throw StreamException("could not seek within file");}
	}

private:

	void open(const std::string& file) {
		fp = fopen(file.c_str(), "rb");
		if (!fp) {throw StreamException("could not open file: " + file);}
	}

	/** the file pointer */
	FILE* fp;

};

}

#endif /* FILEINPUTSTREAM_H_ */
