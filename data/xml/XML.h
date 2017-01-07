#ifndef XML_H
#define XML_H

#include <vector>
#include <string>
#include "../../inc/tinyxml/tinyxml2.h"


namespace K {

	using XMLNode = tinyxml2::XMLNode;
	using XMLElement = tinyxml2::XMLElement;
	using XMLDocument = tinyxml2::XMLDocument;

	enum class XMLType {
		INT,
		FLOAT,
		STRING,
	};

	struct XMLAttribute {
		XMLType type;
		std::string name;

	};


	template <typename T> struct XMLAttr {
		const char* name;
		T val;
		XMLAttr(const char* name) : name(name) {;}
		operator T() const {return val;}
		void deserialize(const XMLElement* el) {throw 1;}
	};

	template <> struct XMLAttr<float> {
		const char* name;
		float val;
		XMLAttr<float>(const char* name) : name(name) {;}
		operator float() const {return val;}
		void deserialize(const XMLElement *el) {this->val = el->FloatAttribute(name);}
	};

	template <> struct XMLAttr<uint16_t> {
		const char* name;
		uint16_t val;
		XMLAttr<uint16_t>(const char* name) : name(name) {;}
		operator uint16_t() const {return val;}
		void operator = (uint16_t val) {this->val = val;}
		void deserialize(const XMLElement *el) {this->val = el->IntAttribute(name);}
	};

	template <> struct XMLAttr<int> {
		const char* name;
		int val;
		XMLAttr<int>(const char* name) : name(name) {;}
		operator int() const {return val;}
		void operator = (int val) {this->val = val;}
		void deserialize(const XMLElement *el) {this->val = el->IntAttribute(name);}
	};

	template <> struct XMLAttr<std::string> {
		const char* name;
		std::string val;
		XMLAttr<std::string>(const char* name) : name(name) {;}
		operator const std::string&() const {return val;}
		void operator = (const std::string& str) {this->val = str;}
		void operator = (const char* str) {this->val = str;}
		void deserialize(const XMLElement *el) {this->val = el->Attribute(name);}
	};



	struct XMLEntity {

	//	static std::vector<XMLAttribute>& getAttributes() {
	//		static std::vector<XMLAttribute> attrs;
	//		return attrs;
	//	}

	//	static void addAttribute(const XMLAttribute& attr) {
	//		getAttributes().push_back(attr);
	//	}

		static void checkRes(const tinyxml2::XMLError err) {
			if (err != tinyxml2::XMLError::XML_NO_ERROR) {
				throw "error: " + std::to_string(err);
			}
		}

		virtual void deserialize(const XMLElement* el) = 0;

	};

	struct XMLRoot : XMLEntity {

		void load(const std::string& file) {
			XMLDocument doc;
			const tinyxml2::XMLError res = doc.LoadFile(file.c_str());
			checkRes(res);
			deserialize(doc.RootElement());
		}

	};


    #define SPLIT(args) \
	    (1)args, (2)args

    #define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__})/sizeof(int))

    #define HEAD(x, ...) x
    #define TAIL(x, ...) __VA_ARGS__

    #define TO_VARIABLE(...) HEAD(__VA_ARGS__) TAIL(__VA_ARGS__)

    #define LE_TEST(structName, ...) \
	    struct structName { \
	        TO_VARIABLE(__VA_ARGS__);\
	    };

    #define XML_ATTRIBUTE(type, name) \
	    K::XMLAttr<type> name = K::XMLAttr<type>(#name)

    #define XML_ENTITY(type, name) \
	    type name

    #define XML_DESERIALIZE(...) \
	    void deserialize(const K::XMLElement* el) {\
	    __VA_ARGS__\
	}

    #define XML_DESERIALIZE_ARG(arg) \
	    arg.deserialize(el);


    #define XML_DESERIALIZE_FIRST_ENTITY(entity) \
	    entity.deserialize(el->FirstChildElement(#entity))
/*
	#define XML_DESERIALIZE_FIRST_ENTITY(entity, nodeName) \
		entity.deserialize(el->FirstChildElement(nodeName))
*/

    #define XML_FOREACH(child, parent) \
	    for(K::XMLElement* child = (K::XMLElement*)parent->FirstChild(); child != nullptr; child = (K::XMLElement*)child->NextSibling())

}

#endif // XML_H
