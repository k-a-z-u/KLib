/*
 * FileOutputStream.h
 *
 *  Created on: Dec 17, 2013
 *      Author: kazu
 */

#ifndef FILEOUTPUTSTREAM_H_
#define FILEOUTPUTSTREAM_H_

#include "OutputStream.h"
#include "../fs/File.h"

namespace K {

/**
 * write data to the given file
 */
class FileOutputStream : public OutputStream {

public:

	/** ctor */
	FileOutputStream(const std::string& file) : fp(nullptr) {
		open(file);
	}

	/** ctor */
	FileOutputStream(const File& file) : FileOutputStream(file.getAbsolutePath()) {
		;
	}

	/** dtor */
	~FileOutputStream() {
		close();
	}

	void write(uint8_t data) override {
		write(&data, 1);
	}

	void write(const uint8_t* data, unsigned int len) override {
		if (!len) {return;}
		if (!fp) {throw "file not open!";}
		fwrite(data, len, 1, fp);
	}

	void flush() override{
		if (!fp) {throw "file not open!";}
		fflush(fp);
	}

	void close() override {
		if (fp) {
			flush();
			fclose(fp);
			fp = nullptr;
		}
	}

private:

	/** open the given file for writing */
	void open(const std::string& file) {
		fp = fopen(file.c_str(), "wb");
		if (!fp) {throw "error while opening file";}
	}

	FILE* fp;

};

}


#endif /* FILEOUTPUTSTREAM_H_ */
