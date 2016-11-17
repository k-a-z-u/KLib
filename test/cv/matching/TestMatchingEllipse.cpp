#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../cv/ImageChannel.h"
#include "../../../cv/matching/EllipseEstimatorMath.h"

using namespace K;

TEST(EllipseEV, ev) {

	std::vector<K::Point2i> pts = {
		K::Point2i(1,6),
		K::Point2i(2,7),
		K::Point2i(3,8),
		K::Point2i(4,9),
		K::Point2i(5,0),
		K::Point2i(6,1),
	};

	EllipseEstimator::Estimation::getParamsEV(pts);

}



#endif


