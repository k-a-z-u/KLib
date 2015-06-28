#ifndef K_DATA_JSON_JSONARRAY_H
#define K_DATA_JSON_JSONARRAY_H

#include <vector>
#include "JSONValue.h"

namespace K {

	/**
	 * represents a JSON-Array holding several sub-object/values
	 */
	class JSONArray {

	private:

		friend class JSONWriter;

		/** all entries within the array */
		std::vector<JSONValue> entries;

	public:

		/** ctor */
		JSONArray() {;}

		/** dtor */
		~JSONArray() {;}

		/** add the given entry into the array */
		void add(JSONValue&& entry) {
			entries.push_back(std::move(entry));
		}

		/** add a null-value */
		void addNull() {
			entries.push_back(JSONValue());
		}

		/** add a boolean-value */
		void addBool(const bool b) {
			entries.push_back(JSONValue(b));
		}

		/** add a double-value for the given key */
		void addDouble(const double d) {
			entries.push_back(JSONValue(d));
		}

		/** add an integer-value for the given key */
		void addInt(const int64_t i) {
			entries.push_back(JSONValue(i));
		}

		/** set a string-value for the given key */
		void addString(const char* str) {
			entries.push_back(JSONValue(str));
		}

		/** set a json-object for the given key */
		void addObject(JSONObject* obj) {
			entries.push_back(JSONValue(obj));
		}

		/** set a json-array for the given key */
		void addArray(JSONArray* arr) {
			entries.push_back(JSONValue(arr));
		}



		/** get the idx-th entry within the array */
		const JSONValue& get(const int idx) {
			return entries.at(idx);
		}

		/** array access operator */
		const JSONValue& operator[] (const int idx) const {
			return entries.at(idx);
		}


		/** get the last value within the array */
		const JSONValue& back() {
			return entries.back();
		}

		/** get the first value within the array */
		const JSONValue& front() {
			return entries.front();
		}

	};

}

#include "JSONObject.h"
#include "JSONArray.h"

/**
 * dunno why i need to add this one here instead of JSONValue.h.
 * putting it there fails to declare JSONArray.h somehow...
 */
K::JSONValue::~JSONValue() {
	switch(type) {
		case EMPTY:
		case BOOLEAN:
		case INT:
		case DOUBLE:
			break;
		case STRING:
			delete[] s; s = nullptr; break;
		case JSON_OBJECT:
			delete obj; obj = nullptr; break;
		case JSON_ARRAY:
			delete arr; arr = nullptr; break;
	}
}

#endif // K_DATA_JSON_JSONARRAY_H
