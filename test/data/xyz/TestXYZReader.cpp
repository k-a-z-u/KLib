#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../data/xyz/XYZFile.h"



using namespace K;

TEST(XYZReader, read) {

	// try to load the object
	XYZFileReader reader(getDataFile("cylinder.xyz"));

	// check number of faces
	ASSERT_EQ(717, reader.getData().vertices.size());



}



#endif
