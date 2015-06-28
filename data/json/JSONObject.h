#ifndef K_DATA_JSON_JSONOBJECT_H
#define K_DATA_JSON_JSONOBJECT_H

#include <string>
#include <unordered_map>
#include <cstdint>

#include "JSONValue.h"
#include "JSONArray.h"


namespace K {


	/**
	 * represents a JSON-object holding several key-value parameters
	 */
	class JSONObject {

	private:

		friend class JSONWriter;

		/** all key-value pairs within this object */
		std::unordered_map<std::string, JSONValue> keyVal;

	public:

		/** set a null-value for the given key */
		void putNull(const std::string& key) {
			keyVal[key] = JSONValue();
		}

		/** set a boolean-value for the given key */
		void put(const std::string& key, const bool b) {
			keyVal[key] = JSONValue(b);
		}	

		/** set a double-value for the given key */
		void put(const std::string& key, const double d) {
			keyVal[key] = JSONValue(d);
		}

		/** set an integer-value for the given key */
		void put(const std::string& key, const int64_t i) {
			keyVal[key] = JSONValue(i);
		}

		/** set a string-value for the given key */
		void put(const std::string& key, const char* str) {
			keyVal[key] = JSONValue(str);
		}

		/** set a json-object for the given key */
		void put(const std::string& key, JSONObject* obj) {
			keyVal[key] = JSONValue(obj);
		}

		/** set a json-array for the given key */
		void put(const std::string& key, JSONArray* arr) {
			keyVal[key] = JSONValue(arr);
		}

		/** set something for the given key */
		void put(const std::string& key, JSONValue&& val) {
			keyVal[key] = std::move(val);
		}

		/** does the object contain a value for the given key? */
		bool containsValue(const std::string& key) {
			return (keyVal.find(key) != keyVal.end());
		}

		/** get the value for the given key. throws an exception if no such value is present */
		const JSONValue& getValue(const std::string& key) {
			auto it = keyVal.find(key);
			if (it == keyVal.end()) {throw "value for key not present";}
			return it->second;
		}

		/** get the double-value behind the given key */
		double getDouble(const std::string& key) {
			return getValue(key).asDouble();
		}

		/** get the boolean-value behind the given key */
		bool getBoolean(const std::string& key) {
			return getValue(key).asBool();
		}

		/** get the integer-value behind the given key */
		int64_t getInt(const std::string& key) {
			return getValue(key).asInt();
		}

		/** get the string-value behind the given key */
		std::string getString(const std::string& key) {
			return getValue(key).asString();
		}

		/** get the json-object behind the given key */
		JSONObject* getObject(const std::string& key) {
			return getValue(key).asObject();
		}

		/** get the json-array behind the given key */
		JSONArray* getArray(const std::string& key) {
			return getValue(key).asArray();
		}

		/** array access by key */
		const JSONValue& operator[] (const char* key) {
			return getValue(key);
		}

	};

}

#endif // K_DATA_JSON_JSONOBJECT_H
