#ifndef HTTPHELPER_H
#define HTTPHELPER_H

#include <string>
#include <algorithm>

namespace K {

	class HttpHelper {

	public:

		/** convert str to lower case */
		static std::string toLower(const std::string& str) {
			std::string tmp = str;
			std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
			return tmp;
		}

		/** convert str to upper case */
		static std::string toUpper(const std::string& str) {
			std::string tmp = str;
			std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
			return tmp;
		}

	};

}

#endif // HTTPHELPER_H
