#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/ImageChannel.h"
#include "../../../cv/matching/MatchingSAD.h"
#include "../../../cv/matching/Matching.h"

using namespace K;

TEST(MatchingSAD, score) {

	ImageChannel img1(3,3);
	img1.zero();
	img1.set(1,1,1);

	ImageChannel img2(5,5);
	img2.zero();
	img2.set(2,2,1);

	MatchingSAD sad(img1, img2);

	ASSERT_EQ(0, sad.getError(Point2i(1,1), Point2i(2,2)));

	ASSERT_EQ(2, sad.getError(Point2i(0,1), Point2i(2,2)));
	ASSERT_EQ(2, sad.getError(Point2i(2,1), Point2i(2,2)));
	ASSERT_EQ(2, sad.getError(Point2i(1,0), Point2i(2,2)));
	ASSERT_EQ(2, sad.getError(Point2i(1,2), Point2i(2,2)));

	ASSERT_EQ(2, sad.getError(Point2i(1,1), Point2i(1,2)));
	ASSERT_EQ(2, sad.getError(Point2i(1,1), Point2i(3,2)));
	ASSERT_EQ(2, sad.getError(Point2i(1,1), Point2i(2,1)));
	ASSERT_EQ(2, sad.getError(Point2i(1,1), Point2i(2,3)));

}

TEST(MatchingSAD, refine) {

	ImageChannel img1(3,3);
	img1.zero();
	img1.set(1,1,1);

	ImageChannel img2(5,5);
	img2.zero();
	img2.set(2,2,1);

	MatchingSAD sad(img1, img2);
	Point2i p2 = Matching::refine(sad, Point2i(1,1), Point2i(0,0));
	ASSERT_EQ(Point2i(2,2), p2);




}

#endif


