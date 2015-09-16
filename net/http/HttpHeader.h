#ifndef K_NET_HTTP_HTTPHEADER_H
#define K_NET_HTTP_HTTPHEADER_H

#include <unordered_map>
#include <string>
#include "HttpHelper.h"
#include "HttpException.h"

namespace K {

	static const std::string EMPTY;

	class HttpHeader {

	private:

		/** key -> value mapping */
		std::unordered_map<std::string, std::string> values;

		/** empty-string (for return-by-ref) */
		//static const std::string EMPTY;

	public:

		/** does the header contain a value for the given key? */
		bool contains(const std::string& key) const {
			const std::string keyLower = HttpHelper::toLower(key);
			return values.find(keyLower) != values.end();
		}

		/** add the given key-value combination */
		void add(const std::string& key, const std::string& value) {
			const std::string keyLower = HttpHelper::toLower(key);
			values[keyLower] = value;
		}

		/** remove the value for the given key (if any) */
		void remove(const std::string& key) {
			const std::string keyLower = HttpHelper::toLower(key);
			values.erase(keyLower);
		}

		/** get the value behind the provided key (if any) */
		const std::string& get(const std::string& key) const {
			const std::string keyLower = HttpHelper::toLower(key);
			auto it = values.find(keyLower);
			return (it == values.end()) ? (EMPTY) : (it->second);
		}

		/** add a header line to the header: "Connection: close\r\n" */
		void addLine(const std::string& line) {
			const size_t pos = line.find(": ");
			if (pos == line.npos) {throw HttpException("invalid header line given: " + line);}
			const std::string key = line.substr(0, pos);
			std::string val = line.substr(pos + 2);
			if (val[val.length()-1] == '\n') {val.pop_back();}
			if (val[val.length()-1] == '\r') {val.pop_back();}
			add(key, val);
		}

		/** get the header as a formatted string */
		std::string getAsString() const {
			std::string ret; ret.reserve(1024);
			for (auto it = values.begin(); it != values.end(); ++it) {
				ret += it->first;
				ret += ": ";
				ret += it->second;
				ret += "\r\n";
			}
			return ret;
		}

	};

}

#endif // K_NET_HTTP_HTTPHEADER_H
