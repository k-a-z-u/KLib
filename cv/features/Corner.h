#ifndef K_CV_FEATURES_CORNER_H
#define K_CV_FEATURES_CORNER_H

namespace K {

	struct Corner {

		float x;
		float y;
		float strength;

		Corner() : x(0), y(0), strength(0) {;}
		Corner(const float x, const float y, const float strength) : x(x), y(y), strength(strength) {;}

	};

}

#endif // K_CV_FEATURES_CORNER_H

