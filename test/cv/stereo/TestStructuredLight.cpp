

#ifdef WITH_TESTS
#ifdef WITH_JPEG

#include "../../Test.h"
#include "../../../cv/stereo/StructuredLight.h"
#include "../../../cv/ImageFactory.h"

#include "../../../cv/filter/Gauss.h"
#include "../../../cv/filter/Normalize.h"
#include "../../../cv/filter/Median.h"
#include "../../../math/statistics/Average.h"

#include <sstream>
using namespace K;
using namespace K::CV;

TEST(Stereo, StructuredLight) {

	const std::string path = "/apps/workspaces/kiste_data/simulation/box2/stripes/";
	const std::string pathCal = "/apps/workspaces/kiste_data/simulation/box2/stripes/calib/";

	const StructuredLight::Dataset dsCal(
		ImageFactory::readJPEG(pathCal + "0.jpg"),
		{
			ImageFactory::readJPEG(pathCal + "1.jpg"),
			ImageFactory::readJPEG(pathCal + "2.jpg"),
			ImageFactory::readJPEG(pathCal + "4.jpg"),
			ImageFactory::readJPEG(pathCal + "8.jpg"),
			ImageFactory::readJPEG(pathCal + "16.jpg"),
			ImageFactory::readJPEG(pathCal + "32.jpg"),
			ImageFactory::readJPEG(pathCal + "64.jpg"),
			ImageFactory::readJPEG(pathCal + "128.jpg"),
			ImageFactory::readJPEG(pathCal + "256.jpg"),
		}
	);
	const StructuredLight::Dataset ds(
		ImageFactory::readJPEG(path + "0.jpg"),
		{
			ImageFactory::readJPEG(path + "1.jpg"),
			ImageFactory::readJPEG(path + "2.jpg"),
			ImageFactory::readJPEG(path + "4.jpg"),
			ImageFactory::readJPEG(path + "8.jpg"),
			ImageFactory::readJPEG(path + "16.jpg"),
			ImageFactory::readJPEG(path + "32.jpg"),
			ImageFactory::readJPEG(path + "64.jpg"),
			ImageFactory::readJPEG(path + "128.jpg"),
			ImageFactory::readJPEG(path + "256.jpg"),
		}
	);

	StructuredLight sl;
	sl.calibrate(dsCal);
	ImageChannel depth = sl.getDepth(ds, 40.0);
	depth = MedianFilter::apply(depth, 3, 1);

	depth.forEachModify([&] (const int x, const int y, const float v) {
		K::Average<float> avg;
		if (v == 0) {return v;}
		const int s = 1;
		for (int x1 = x-s; x1 <= x+s; ++x1) {
			for (int y1 = y-s; y1 <= y+s; ++y1) {
				const float val = depth.getClamped(x1,y1);
				if (val > 0 && std::abs(v-val) < 0.04) {avg.add(val);}
			}
		}
		return (avg.size() > 0) ? (avg.get()) : (0);
	});

	Normalize::inplace(depth);



	ImageFactory::writeJPEG("/tmp/nyan.jpg", depth);


}


#endif

#endif
