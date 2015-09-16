#ifndef K_DATA_JSON_JSONVALUE_H
#define K_DATA_JSON_JSONVALUE_H

#include <string>
#include <cstdint>
#include "JSONTypes.h"

namespace K {

	class JSONArray;
	class JSONObject;

	/**
	 * describes a (variant) JSON-value.
	 * a value is one of:
	 *		null,boolean,integer,double,string,json-object,json-array
	 */
	class JSONValue {

		friend class JSONWriter;

		/** the type of contained value (variant) */
		JSONValueType type;

		/** union to access the value (depends on above type) */
		union {

			bool b;
			double d;
			int64_t i;

			char* s;
			JSONObject* obj;
			JSONArray* arr;

		};

	public:

		/** ctor: null-value */
		explicit JSONValue() : type(JSONValueType::EMPTY) {;}
		/** ctor: boolean-value */
		explicit JSONValue(const bool b) : type(JSONValueType::BOOLEAN), b(b) {;}
		/** ctor: double-value */
		explicit JSONValue(const double d): type(JSONValueType::DOUBLE), d(d) {;}
		/** ctor: integer-value */
		explicit JSONValue(const int64_t i) : type(JSONValueType::INT), i(i) {;}
		/** ctor: json-object */
		explicit JSONValue(JSONObject* obj) : type(JSONValueType::JSON_OBJECT), obj(obj) {;}
		/** ctor: json-array */
		explicit JSONValue(JSONArray* arr) : type(JSONValueType::JSON_ARRAY), arr(arr) {;}
		/** ctor: string-value */
		explicit JSONValue(const std::string& str) : type(JSONValueType::STRING) {
			s = new char[str.size()+1];
			strcpy(s, str.c_str());
		}

		/** dtor */
		~JSONValue();

		/** hidden copy ctor */
		JSONValue(const JSONValue& o) = delete;

		/** move ctor */
		JSONValue(JSONValue&& o) {
			this->type = o.type;
			this->d = o.d;
			o.i = 0;
		}

		/** move assignment operator */
		JSONValue& operator= (JSONValue&& o) {
			this->type = o.type;
			this->d = o.d;
			o.i = 0;
			return *this;
		}

		bool asBool() const {return b;}
		double asDouble() const {return d;}
		int64_t asInt() const {return i;}
		std::string asString() const {return std::string(s);}
		JSONObject* asObject() const {return obj;}
		JSONArray* asArray() const {return arr;}

	};

}

#endif // K_DATA_JSON_JSONVALUE_H
