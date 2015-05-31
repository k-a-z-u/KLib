#ifndef K_NET_RTMP_AMF_AMFENTRIES_H
#define K_NET_RTMP_AMF_AMFENTRIES_H

#include <vector>
#include <iostream>

namespace K {

	/** entry base class */
	struct AMFEntry {
		virtual ~AMFEntry() {;}
		virtual void print(std::ostream& o) const = 0;
		friend std::ostream& operator << (std::ostream& out, const AMFEntry& e) {e.print(out); return out;}
	};

	/** string entry */
	struct AMFEntryString : AMFEntry {
		std::string str;
		AMFEntryString(const std::string& str) : str(str) {;}
		void print (std::ostream& out) const {out << "string: " << str;}
	};

	/** number entry */
	struct AMFEntryNumber : AMFEntry {
		double nr;
		AMFEntryNumber(const double nr) : nr(nr) {;}
		void print(std::ostream& out) const {out << "number: " << nr;}
	};

	/** null entry */
	struct AMFEntryNull : AMFEntry {
		void print(std::ostream& out) const {out << "null";}
	};


	/** boolean entry */
	struct AMFEntryBoolean : AMFEntry {
		bool b;
		AMFEntryBoolean(const bool b) : b(b) {;}
		void print(std::ostream& out) const {out << "bool: " << b;}
	};

	/** key -> value entry */
	struct AMFEntryKeyVal : AMFEntry {
		std::string key;
		AMFEntry* val;
		AMFEntryKeyVal(const AMFEntryKeyVal& o) = delete;
		AMFEntryKeyVal(AMFEntryKeyVal&& o) {this->key = o.key; this->val = o.val; o.val = nullptr;}
		AMFEntryKeyVal(const std::string& key, AMFEntry* val) : key(key), val(val) {;}
		~AMFEntryKeyVal() {delete val;}
		void print(std::ostream& out) const {out << key << ": " << *val;}
	};

	/** object entry */
	struct AMFEntryObject : public AMFEntry {

		/** all contained entries */
		std::vector<AMFEntryKeyVal> entries;

		AMFEntryObject() {;}

		void print(std::ostream& out) const {
			for (const AMFEntryKeyVal& e : entries) {out << e << std::endl;}
		}

		/** get the (first) AMFEntry (value) behind the given key (if any) */
		AMFEntry* getValueForKey(const std::string& key) {
			for (const AMFEntryKeyVal& e : entries) {if (e.key == key) {return e.val;}}
			return nullptr;
		}
	};


	/** ecma array */
	struct AMFEcmaArray : AMFEntry {

		/** all contained entries */
		std::vector<AMFEntryKeyVal> entries;

		AMFEcmaArray() {;}

		void print(std::ostream& out) const {
			for (const AMFEntryKeyVal& e : entries) {out << e << std::endl;}
		}

	};



	struct AMFResult {

		/** all contained entries */
		std::vector<AMFEntry*> entries;

		AMFResult() {;}
		AMFResult(const AMFResult& o) = delete;
		AMFResult(AMFResult&& o) {this->entries = std::move(o.entries);}
		~AMFResult() { for (AMFEntry* e : entries) {delete e;} entries.clear(); }

		friend std::ostream& operator << (std::ostream& out, const AMFResult& r) {
			for (const AMFEntry* e : r.entries) { out << (*e) << std::endl; }
			return out;
		}

		/** search within all AMFObjectEntries for a key */
		AMFEntry* getValueForKey(const std::string& key) {
			for (AMFEntry* e : entries) {
				AMFEntryObject* eo = dynamic_cast<AMFEntryObject*>(e);
				if (eo) {
					AMFEntry* res = eo->getValueForKey(key);
					if (res) {return res;}
				}
			}
			return nullptr;
		}

	};

}
#endif // K_NET_RTMP_AMF_AMFENTRIES_H
