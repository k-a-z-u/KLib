/*
 * MyString.h
 *
 *  Created on: 03.08.2012
 *      Author: Frank Ebner
 */

#ifndef MYSTRING_H_
#define MYSTRING_H_

#include <string.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

namespace K {

class String {


public:

	/** trim a string */
	static std::string trim(std::string& str) {
		std::string::size_type pos = str.find_last_not_of(' ');
		if(pos != std::string::npos) {
			str.erase(pos + 1);
			pos = str.find_first_not_of(' ');
			if(pos != std::string::npos) str.erase(0, pos);
		}
		else str.erase(str.begin(), str.end());
		return str;
	}

	/** convert string to int */
	static int toInt(const std::string& str) {
		return atoi( str.c_str() );
	}

	/** convert int to string */
	static std::string fromInt(int i) {
		std::stringstream ss;
		ss << i;
		return ss.str();
	}

	/** convert string to float */
	static float toFloat(const std::string& str) {
		return (float) atof( str.c_str() );
	}

	/** convert float to string */
	static std::string fromFloat(float f) {
		std::stringstream ss;
		ss << f;
		return ss.str();
	}


	/** replace all occurences of needle in haystack with replacement */
	static void replaceChar(std::string& haystack, char needle, char replacement) {
		std::replace(haystack.begin(), haystack.end(), needle, replacement);
	}

	/** replace all occurences of needle in haystack with replacement */
	static void replace(std::string& haystack, const std::string& needle, const std::string& replacement) {
		size_t startPos = 0;
		while((startPos = haystack.find(needle, startPos)) != std::string::npos) {
			haystack.replace(startPos, needle.length(), replacement);
			startPos += replacement.length();
		}
	}

	/** check if string starts with something */
	static bool startsWith(const std::string& str, const std::string& other) {
		return str.compare(0, other.length(), other) == 0;
	}

	/** check if string ends with something */
	static bool endsWith(const std::string& str, const std::string& ending) {
		return (str.compare(str.length() - ending.length(), ending.length(), ending) == 0);
	}

	/** remove all whitespaces */
	static void removeSpaces(std::string& str) {
		str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
	}

	/** remove all occurences of char */
	static void removeChar(std::string& str, char c) {
		str.erase (std::remove(str.begin(), str.end(), c), str.end());
	}

	/** remove all occurences of chars */
	static void removeChars(std::string& str, const char chars[]) {
		size_t len = strlen(chars);
		for (size_t i = 0; i < len; ++i) {
			str.erase (std::remove(str.begin(), str.end(), chars[i]), str.end());
		}
	}

	/** split a string using the provided char */
	static std::vector<std::string> split(const std::string& str, char c) {
		std::vector<std::string> vec;
		split(str, vec, c);
		return vec;
	}

	/** split a string using the provided char */
	static size_t split(const std::string& str, std::vector<std::string>& strs, char ch) {

		size_t pos = str.find( ch );
		size_t initialPos = 0;
		strs.clear();

		// Decompose statement
		while( pos != std::string::npos ) {
			strs.push_back( str.substr( initialPos, pos - initialPos ) );
			initialPos = pos + 1;
			pos = str.find( ch, initialPos );
		}

		// Add the last one
		strs.push_back( str.substr( initialPos, std::min( pos, str.size() ) - initialPos + 1 ) );

		return strs.size();
	}

};

}

#endif /* MYSTRING_H_ */
