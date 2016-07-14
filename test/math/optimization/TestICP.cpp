#ifdef WITH_TESTS

#include "../../Test.h"

#include "../../../math/optimization/ICP.h"
#include "../../../geo/Matrix.h"

namespace K {

	TEST(ICP, calc) {

		// generate starting points
		std::vector<Point3f> set1;
		for (int i = 0; i < 9; ++i) {
			const float x = (float) (rand() % 64);
			const float y = (float) (rand() % 64);
			const float z = (float) (rand() % 64);
			set1.push_back(Point3f(x, y, z));
		}

		// rotate them
		Mat4f mat = MatrixHelper::getRotation(Vec3f(1,2,3));
		std::vector<Point3f> set2;
		for (Point3f p1 : set1) {
			const Point3f p2 = mat * p1;
			set2.push_back(p2);
		}

		// estimate the rotation matrix using ICP
		Mat4f imat = ICP::solve(set1, set2);

		// calculate again
		std::vector<Point3f> set3;
		for (Point3f p1 : set1) {
			const Point3f p2 = imat * p1;
			set3.push_back(p2);
		}


		// orig-ROT*p must be near ICP-rot*p
		for (int i = 0; i < (int) set1.size(); ++i) {
			ASSERT_NEAR(0, set2[i].getDistance(set3[i]), 0.01);
		}



		int i = 0; (void) i;

	}


}

#endif
