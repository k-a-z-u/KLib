

#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../data/json/JSONReader.h"
#include "../../../data/json/JSONWriter.h"
#include <sstream>
using namespace K;

TEST(JSON, read_ok) {

	std::stringstream ss;
	JSONReader reader;
	JSONWriter writer(ss, false);

	JSONValue e0 = reader.parse("[]");
	ss.str(""); writer.write(e0);
	ASSERT_EQ("[]", ss.str());

	JSONValue e1 = reader.parse("{}");
	ss.str(""); writer.write(e1);
	ASSERT_EQ("{}", ss.str());

	JSONValue e2 = reader.parse("[ {},	{}, {},{}]");
	ss.str(""); writer.write(e2);
	ASSERT_EQ("[{},{},{},{}]", ss.str());

	JSONValue e3 = reader.parse("[ { }, [ {},{},{} ] ]");
	ss.str(""); writer.write(e3);
	ASSERT_EQ("[{},[{},{},{}]]", ss.str());

	JSONValue e4 = reader.parse("{\"a\":{}}");
	ss.str(""); writer.write(e4);
	ASSERT_EQ("{\"a\":{}}", ss.str());

	JSONValue e5 = reader.parse("{ \"a\" : [] }");
	ss.str(""); writer.write(e5);
	ASSERT_EQ("{\"a\":[]}", ss.str());

	JSONValue e6 = reader.parse("{ \"a\" : null }");
	ss.str(""); writer.write(e6);
	ASSERT_EQ("{\"a\":null}", ss.str());

	JSONValue e7 = reader.parse(" { } ");
	ss.str(""); writer.write(e7);
	ASSERT_EQ("{}", ss.str());

	JSONValue e8 = reader.parse("[ null , true, false ,1, 1.1] ");
	ss.str(""); writer.write(e8);
	ASSERT_EQ("[null,true,false,1,1.1]", ss.str());

	JSONValue e10 = reader.parse("[ {\"xyz\":133.7, \"abc\":\"11\\\"1\"},	[ { }, [ {},{},{} ] ]	,	[{\"a\":true}], [{\"b\":true}, {\"b\":false}] ]");
	ss.str(""); writer.write(e10);
	ASSERT_EQ("[{\"abc\":\"11\\\"1\",\"xyz\":133.7},[{},[{},{},{}]],[{\"a\":true}],[{\"b\":true},{\"b\":false}]]", ss.str());

}

TEST(JSON, read_err) {

	std::stringstream ss;
	JSONReader reader;

	ASSERT_ANY_THROW(reader.parse("[,]"));
	ASSERT_ANY_THROW(reader.parse("[],"));
	ASSERT_ANY_THROW(reader.parse("{a:1}"));

}

TEST(JSON, read) {

	JSONReader reader;
	JSONValue val = reader.parse("{\"a\": 1337, \"b\": false, \"c\":null, \"d\":[1,2,3], \"e\":{\"abc\":\"xyz\"} }");
	JSONObject* obj = val.asObject();

	ASSERT_EQ(1337, obj->getInt("a"));
	ASSERT_EQ(false, obj->getBoolean("b"));
	ASSERT_TRUE(obj->containsValue("c"));
	ASSERT_EQ(1, obj->getArray("d")->get(0).asInt());
	ASSERT_EQ(2, obj->getArray("d")->get(1).asInt());
	ASSERT_EQ(3, obj->getArray("d")->get(2).asInt());
	ASSERT_EQ("xyz", obj->getObject("e")->getString("abc"));
}

TEST(JSON, create) {

	JSONArray arr;
	std::stringstream ss;
	JSONWriter writer(ss, false);

	arr.addInt(1337);
	arr.addDouble(13.37);
	arr.addString("1337");
	arr.addNull();
	arr.addBool(false);

	writer.write(arr);
	std::cout << ss.str() << std::endl;
	ASSERT_TRUE(true);

}

#endif
