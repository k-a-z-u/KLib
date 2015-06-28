#ifndef K_DATA_JSON_JSONWRITER_H
#define K_DATA_JSON_JSONWRITER_H

#include "JSONArray.h"
#include "JSONObject.h"
#include <ostream>

namespace K {

	class JSONWriter {

	private:

		/** the stream to write to */
		std::ostream& out;

		/** whether to use pretty-printing */
		bool prettyPrint;

		/** the char to use for indentation (pretty printing) */
		const char indent;


	public:

		/** ctor */
		JSONWriter(std::ostream& out, bool prettyPrint = true, const char indent = '\t') :
			out(out), prettyPrint(prettyPrint), indent(indent) {
			;
		}

		/** write the given element into the configured output stream */
		void write(JSONValue& e) {
			switchEl("", e, 0);
		}

		/** write the given json-array into the configured output stream */
		void write(const JSONArray& arr) {
			writeArray("", arr, 0);
		}

		/** write the given json-object into the configured output stream */
		void write(const JSONObject& obj) {
			writeObject("", obj, 0);
		}

	private:

		void switchEl(const std::string& key, const JSONValue& e, const int lvl) const {
			switch (e.type) {
				case EMPTY:			writeNull(key, lvl); break;
				case BOOLEAN:		writeBool(key, e, lvl); break;
				case DOUBLE:		writeDouble(key, e, lvl); break;
				case INT:			writeInt(key, e, lvl); break;
				case STRING:		writeString(key, e, lvl); break;
				case JSON_OBJECT:	writeObject(key, *e.obj, lvl); break;
				case JSON_ARRAY:	writeArray(key, *e.arr, lvl); break;
			}
		}

		/** write x tabs (for pretty print) */
		void writeTabs(const int lvl) const {
			if (!prettyPrint) {return;}
			for (int i = 0; i < lvl; ++i) {out << indent;}
		}

		/** write the key-name */
		void writeKey(const std::string& key, const int lvl) const {
			writeTabs(lvl);
			if (!key.empty()) {
				out << '"' << key << "\":";
				if (prettyPrint) {out << ' ';}
			}
		}

		/** write a null-value */
		void writeNull(const std::string& key, const int lvl) const {
			writeKey(key, lvl);
			out << "null";
		}

		/** write a key->value boolean */
		void writeBool(const std::string& key, const JSONValue& e, const int lvl) const {
			writeKey(key, lvl);
			out << ((e.b) ? ("true") : ("false"));
		}

		/** write a key->value double */
		void writeDouble(const std::string& key, const JSONValue& e, const int lvl) const {
			writeKey(key, lvl);
			out << e.d;
		}

		/** write a key->value int */
		void writeInt(const std::string& key, const JSONValue& e, const int lvl) const {
			writeKey(key, lvl);
			out << e.i;
		}

		/** write a key->value string */
		void writeString(const std::string& key, const JSONValue& e, const int lvl) const {
			writeKey(key, lvl);
			out << '"' << escape(e.s) << '"';
		}

		/** write a complete json-object */
		void writeObject(const std::string& key, const JSONObject& obj, int lvl) const {
			writeKey(key, lvl);	out << "{";
			const int num = (int) obj.keyVal.size();
			if (num && prettyPrint) {out << "\n"; ++lvl;}
			int i = 0;
			for (auto& it : obj.keyVal) {
				switchEl(it.first, it.second, lvl);
				if (++i < num) {out << ",";}
				if (prettyPrint) {out << "\n";}
			}
			if (num && prettyPrint) {writeTabs(--lvl);}	out << "}";
		}

		/** write a complete json-array */
		void writeArray(const std::string& key, const JSONArray& arr, int lvl) const {
			writeKey(key, lvl);	out << "[";
			const int num = (int) arr.entries.size();
			if (num && prettyPrint) {out << "\n"; ++lvl;}
			int i = 0;
			for (auto& el : arr.entries) {
				switchEl("", el, lvl);
				if (++i < num) {out << ",";}
				if (prettyPrint) {out << "\n";}
			}
			if (num && prettyPrint) {writeTabs(--lvl);}	out << "]";
		}

		/** escape " within the given string */
		std::string escape(const char* str) const {
			std::string res = std::string(str);
			size_t startPos = 0;
			while((startPos = res.find('"', startPos)) != std::string::npos) {
				res.replace(startPos, 1, "\\\"");
				startPos += 2;
			}
			return res;
		}

	};

}

#endif // K_DATA_JSON_JSONWRITER_H
