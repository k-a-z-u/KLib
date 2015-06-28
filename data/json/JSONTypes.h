#ifndef K_DATA_JSON_JSONTYPES_H
#define K_DATA_JSON_JSONTYPES_H

namespace K {

	/** enum of all supported value-types within JSON */
	enum JSONValueType {
		EMPTY,
		BOOLEAN,
		DOUBLE,
		INT,
		STRING,
		JSON_OBJECT,
		JSON_ARRAY,
	};

}

#endif // K_DATA_JSON_JSONTYPES_H
