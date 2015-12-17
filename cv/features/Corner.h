#ifndef K_CV_FEATURES_CORNER_H
#define K_CV_FEATURES_CORNER_H

namespace K {

	struct Corner {

		int x;
		int y;
		float strength;

		Corner() : x(0), y(0), strength(0) {;}
		Corner(const int x, const int y, const float strength) : x(x), y(y), strength(strength) {;}

	};

}

#endif // K_CV_FEATURES_CORNER_H

