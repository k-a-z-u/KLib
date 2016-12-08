#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../data/obj/ObjectFile.h"

using namespace K;

TEST(ObjReader, read) {

	// try to load the object
	ObjFileReader reader;
	reader.readFile(getDataFile("cylinder.obj"));

	// check number of faces
	ASSERT_EQ(32, reader.getData().faces.size());

	// check one face
	ObjFileReader::Face face31 = reader.getData().faces[31];
	ASSERT_EQ(face31.vnt[0].vertex, ObjFileReader::Vec3(0,30,0));
	ASSERT_EQ(face31.vnt[2].vertex, ObjFileReader::Vec3(20,30,0));

}



#endif
