#ifndef K_DATA_TOKENIZER_H
#define K_DATA_TOKENIZER_H

#include <string>

namespace K {

	/**
	 * split an input-file into various tokens
	 */
	class Tokenizer {

		std::string str;
		char ignore = '\0';
		size_t pos;

	public:

		/** ctor */
		Tokenizer(const std::string& str, const char ignore = '\0') : str(str), ignore(ignore), pos(0) {;}

		/** get the next token */
		std::string getToken(const char delim, const bool skipEmpty = true) {

			// skip empty tokens
			if (skipEmpty) {skip(delim);}

			// find end of next token
			for (size_t i = pos; i < str.length(); ++i) {
				if (skip(str[i], delim)) {
					const std::string token = str.substr(pos, i-pos);
					pos = i + 1;
					if (skipEmpty) {skip(delim);}
					return token;
				}
			}

			// last token
			const std::string token = str.substr(pos);
			pos = str.length();
			return token;

		}

		/** more tokens? */
		bool hasNext() const {
			return pos < str.length();
		}

	private:

		/** skip this char? */
		bool skip(const char c, const char delim) const {
			return (c == delim) || (c == ignore);
		}

		void skip(const char delim) {
			while(skip(str[pos], delim) && pos < str.length()) {++pos;}
		}

	};

}

#endif // K_DATA_TOKENIZER_H
