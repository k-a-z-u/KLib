/*
 * File.h
 *
 *  Created on: 02.09.2012
 *      Author: Frank Ebner
 */

#ifndef FILE_H_
#define FILE_H_

#include <string>
#include <algorithm>
#include <cstring>
#include <vector>
#include <iostream>
#include <sys/stat.h>


#include <stdio.h>

#ifdef __MINGW32__
#include <dirent.h>
#include <direct.h>
#define getCurrentDir _getcwd
#define NEEDED_SLASH '\\'
#define OTHER_SLASH '/'
#elif _WIN32
#define NOMINMAX
#include "dirent.h"
#include <direct.h>
#define getCurrentDir _getcwd
#define NEEDED_SLASH '/'
#define OTHER_SLASH '\\'
#else
#include <dirent.h>
#include <unistd.h>
#define getCurrentDir getcwd
#define NEEDED_SLASH '/'
#define OTHER_SLASH '\\'
#endif


namespace K {

/** exceptions within the file-class */
class FileException : public std::exception {

public:

	/** ctor (message) */
	explicit FileException(const std::string& msg) : msg(msg) {
		;
	}

	/** exception message */
	virtual const char* what() const throw() override {
		return msg.c_str();
	}

private:

	/** error message */
	std::string msg;

};




class File {

public:

	/** empty ctor */
	File() : fileName(getWorkingDir()) {
		//this->fileName = getWorkingDir();
		unify(this->fileName);
	}

	/** copy ctor */
	File(const File& file) : fileName(file.getAbsolutePath()) {
		//this->fileName = file.getAbsolutePath();
		unify(this->fileName);
	}

	/** ctor from file name */
	File(const std::string& fileName) : fileName() {
		if (fileName[0] == NEEDED_SLASH || fileName[1] == ':') {
			this->fileName = fileName;
		} else {
			this->fileName = getWorkingDir() + NEEDED_SLASH + fileName;
		}
		unify(this->fileName);
	}

	/** ctor file in folder */
	File(const File& parent, const std::string& fileName) : fileName() {
		this->fileName = parent.getAbsolutePath() + NEEDED_SLASH + fileName;
		unify(this->fileName);
	}

	/** dtor */
	~File() {;}

	/** get the file's name only */
	std::string getName() const {
		size_t pos = fileName.find_last_of(NEEDED_SLASH);
		if (pos == fileName.npos) {return "";}
		return fileName.substr(pos+1);
	}

	/** get the file's extension only (like "xml") */
	std::string getExtension() const {
		size_t pos1 = fileName.find_last_of(NEEDED_SLASH);
		size_t pos2 = fileName.find_last_of(".");
		if (pos2 == fileName.npos || pos2 < pos1) {return "";}
		return fileName.substr(pos2+1);
	}

	/** get the file's extension only, supporting multiple extensions (like "xml.gz") */
	std::string getExtensionMulti() const {
		size_t pos1 = fileName.find_last_of(NEEDED_SLASH);
		size_t pos2 = fileName.find_first_of(".", pos1);
		if (pos2 == fileName.npos || pos2 < pos1) {return "";}
		return fileName.substr(pos2+1);
	}

	/** get the file's name without the extension */
	std::string getNameWithoutExt() const {
		std::string name = getName();
		size_t len = getExtension().length();
		return name.substr(0, name.length() - (len ? len+1 : 0) );
	}

	/** get the parent (folder) of this file/folder */
	File getParent() const {
		size_t pos = fileName.find_last_of(NEEDED_SLASH);
		if (pos == std::string::npos) {return fileName;}		// fixme OK?
		std::string newFileName = fileName.substr(0, pos);
		return File(newFileName);
	}

	/** get the working-directory */
	static std::string getWorkingDir() {
		char cwd[FILENAME_MAX] = {0};
		if (!getCurrentDir(cwd, sizeof(cwd))) {return std::string();}
		std::string dir = std::string(cwd);
		File::unify(dir);
		return dir;
	}

	/** get the absolute path of this file */
	std::string getAbsolutePath() const {
		return fileName;
	}

	/** get the relative (to working directory) path of this file */
	std::string getRelativePath() const {
		return fileName.substr( getWorkingDir().length() );
	}

	/** get the file's size */
	size_t length() const {
		struct stat status;
		stat( getAbsolutePath().c_str(), &status );
		return status.st_size;
	}


	/** returns true if this file exists */
	bool exists() const {
		struct stat buf;
		int retCode = stat(getAbsolutePath().c_str(), &buf);
		return (retCode != -1) ? true : false;
	}

	/** returns true if this is a folder */
	bool isFolder() const {
		struct stat buf;
		int retCode = stat(getAbsolutePath().c_str(), &buf);
		if (retCode == -1) {return false;}
		return S_ISDIR(buf.st_mode);
	}

	/** returns true if this is a file */
	bool isFile() const {
		struct stat buf;
		int retCode = stat(getAbsolutePath().c_str(), &buf);
		if (retCode == -1) {return false;}
		return !S_ISDIR(buf.st_mode);
	}

	/** create this directory (all parent directories must already be present!) */
	void mkdir() {
		#ifdef __linux__
			::mkdir(getAbsolutePath().c_str(), 0744);
		#else
			_mkdir(getAbsolutePath().c_str());
		#endif
	}

	/** get all files/folders within this folder */
	void listFiles(std::vector<File>& lst) {

		DIR* pDIR;
		pDIR = opendir(getAbsolutePath().c_str());
		if( pDIR == NULL ) {return;}

		struct dirent* entry;
		while( (entry = readdir(pDIR)) != NULL ){
			if (strcmp(entry->d_name, ".") == 0) {continue;}
			if (strcmp(entry->d_name, "..") == 0) {continue;}
			File f(*this, std::string(entry->d_name));
			lst.push_back(f);
		}

		// cleanup
		closedir(pDIR);

	}

	/** quick&dirty write-to-file function */
	void write(uint8_t* data, uint32_t len) {
		FILE* handle = fopen(getAbsolutePath().c_str(), "wb");
		if (!handle) {throw FileException("could not open file '" + getAbsolutePath() + "' for writing");}
		fwrite(data, len, 1, handle);
		fclose(handle);
	}

	/** delete this file */
	void deleteMe() {
		if (isFolder()) {
			throw FileException("tried to delete a folder: " + getAbsolutePath());
		}
		if( remove( getAbsolutePath().c_str() ) != 0 ) {
			throw FileException("error while deleting '" + getAbsolutePath() + "'");
		}
	}

	/** returns a temporary file with unique name */
	static File getTempFile() {
#ifdef __MINGW32__
        char buffer [L_tmpnam];
        tmpnam (buffer);
#elif _WIN32
		char buffer[L_tmpnam_s];
		errno_t err = tmpnam_s(buffer, L_tmpnam_s);
		if(err) {throw FileException("error while getting a temporary filename");}
#else
		char buffer [L_tmpnam];
		tmpnam (buffer);
#endif
		return File(std::string(buffer));
	}


private:

	/** convert slashes etc. */
	static void unify(std::string& file) {
		replaceChar(file, OTHER_SLASH, NEEDED_SLASH);
	}

	/** replace needle with replacement within str */
	static void replaceChar(std::string& str, char needle, char replacement) {
		std::replace(str.begin(), str.end(), needle, replacement);
	}

	/* attributes */
	std::string fileName;

};


}

#endif /* FILE_H_ */
