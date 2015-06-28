#ifndef K_DATA_JSON_JSONREADER_H
#define K_DATA_JSON_JSONREADER_H

#include "JSONArray.h"
#include "JSONObject.h"
#include <cassert>

namespace K {

	/** exception handling within the reader */
	class JSONReaderException : public std::exception {
	private:
		std::string msg;
	public:
		JSONReaderException(const std::string& msg) : msg(msg) {;}
		JSONReaderException(const char* msg) : msg(msg) {;}
		const char* what() const throw() {return msg.c_str();}
	};

	/** very simple "tokenizer" that uses chars as tokens */
	struct Reader {

		const char* start;
		const char* str;
		const int len;

		/** ctor */
		Reader(const std::string& str) : start(str.c_str()), str(str.c_str()), len((int)str.length()) {;}

		/** assert the next available char is c and hereafter consume it */
		void consume(const char c) {
			if (str[0] != c) {
				throw JSONReaderException(std::string("found unexpected token: expected '") + c + "' got '" + str[0] + "'");
			}
			++str;}

		/** consume the next available char. no matter what it is */
		char consume() {char c = str[0]; ++str; return c;}

		/** if the next available char is c, consume it and return true, else keep it and return false */
		bool tryConsume(const char c) {
			if (str[0] == c) {++str; return true;} else {return false;}
		}

		/** peek into the next available char */
		char peek() const {return str[0];}

		/** peek whether the next available char is c */
		bool peek(const char c) {return peek() == c;}

		/** is the buffer empty? */
		bool isEmpty() const {return str == (start + len);}

	};



	class JSONReader {

	public:

		/** parse the given input data */
		JSONValue parse(const std::string& str) {
			Reader r(str);
			JSONValue res = switchOA(r);

			// the buffer must now be empty. else there is suspicious data at the end
			if (!r.isEmpty()) {
				throw JSONReaderException(std::string("found unexpected trailing data:\n") + r.str);
			}

			return res;

		}

	private:

		/** decide whether the next object is a JSONArray or a JSONObject */
		JSONValue switchOA(Reader& r) const {
			skipWhitespaces(r);
			if (r.peek() == '[') {return parseArray(r);}
			if (r.peek() == '{') {return parseObject(r);}
			else {
				throw JSONReaderException(std::string("found unexpected token. expected '[' or '{' but got '") + r.peek() + '\'');
			}
		}

		/** parse and return a JSONArray */
		JSONValue parseArray(Reader& r) const {
			r.consume('[');
			JSONArray* arr = new JSONArray();
			try {
				skipWhitespaces(r);
				if (!r.tryConsume(']')) {
					do {
						skipWhitespaces(r);
						arr->add(getValue(r));
						skipWhitespaces(r);
					} while (r.tryConsume(','));
					r.consume(']');
				}
			} catch (...) {
				delete arr;
				throw;
			}
			skipWhitespaces(r);
			return JSONValue(arr);
		}

		/** parse and return a JSONObject */
		JSONValue parseObject(Reader& r) const {
			r.consume('{');
			JSONObject* obj = new JSONObject();
			skipWhitespaces(r);
			try {
				if (!r.tryConsume('}')) {
					do {
						skipWhitespaces(r);
						std::string key = getKey(r);
						skipWhitespaces(r);
						r.consume(':');
						skipWhitespaces(r);
						JSONValue val = getValue(r);
						skipWhitespaces(r);
						obj->put(key, std::move(val));
					} while(r.tryConsume(','));
					r.consume('}');
				}
			} catch (...) {
				delete obj;
				throw;
			}

			skipWhitespaces(r);
			return JSONValue(obj);
		}

		/** skip all kinds of whitespaces */
		void skipWhitespaces(Reader& r) const {
			while (r.tryConsume(' ') || r.tryConsume('\t') || r.tryConsume('\r') || r.tryConsume('\n')) {;}
		}

		/** parse a json-key */
		std::string getKey(Reader& r) const {
			r.consume('"');
			const char* start = r.str;
			while(true) {
				++r.str;
				if (r.peek() == '"') {break;}
			}
			std::string key(start, r.str - start);
			r.consume('"');
			return key;
		}

		/** parse a json-value */
		JSONValue getValue(Reader& r) const {
			switch (r.str[0]) {
				case 't': case 'f':
					return getBoolean(r);
				case 'n':
					return getNull(r);
				case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
					return getNumber(r);
				case '"':
					return getString(r);
				case '{':
					return parseObject(r);
				case '[':
					return parseArray(r);
				default: throw JSONReaderException(std::string("expected one of boolean/int/double/string/object/array but got\n") + std::string(r.str));
			}
		}

		/** parse and consume a null-value */
		JSONValue getNull(Reader& r) const {
			if (r.peek() != 'n') {throw JSONReaderException("expected 'null' but got\n" + std::string(r.str));}
			r.consume('n'); r.consume('u'); r.consume('l'); r.consume('l');
			return JSONValue();
		}

		/** parse and consume a boolean-value */
		JSONValue getBoolean(Reader& r) const {
			if (r.peek() == 't') {r.consume('t'); r.consume('r'); r.consume('u'); r.consume('e'); return JSONValue(true);}
			if (r.peek() == 'f') {r.consume('f'); r.consume('a'); r.consume('l'); r.consume('s'); r.consume('e'); return JSONValue(false);}
			throw JSONReaderException("expected one of true/false but got\n" + std::string(r.str));
		}

		/** parse and consume a number-value (int/double) */
		JSONValue getNumber(Reader& r) const {
			bool isFloat = false;
			std::string str;
			while (true) {
				if		(r.peek() == '.')						{isFloat = true; str += r.consume();}
				else if	(r.peek() >= '0' && r.peek() <= '9')	{str += r.consume();}
				else											{break;}
			}
			return (isFloat) ? JSONValue(std::stod(str)) : JSONValue(std::stol(str));
		}

		/** parse and consume a string-value */
		JSONValue getString(Reader& r) const {
			r.consume('"');
			bool escaped = false;
			std::string str;
			while (true) {
				if		(r.peek('\\'))	{escaped = true; r.consume();}
				else if	(r.peek('"'))	{
					if (escaped)	{escaped = false; str += r.consume();}
					else			{break;}
				}
				else							{str += r.consume();}
			}
			r.consume('"');
			return JSONValue(str);
		}

	};

}

#endif // K_DATA_JSON_JSONREADER_H
