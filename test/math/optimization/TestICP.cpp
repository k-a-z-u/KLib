#ifdef WITH_TESTS

#include "../../Test.h"

#include "../../../math/optimization/ICP.h"
#include "../../../geo/Matrix.h"

namespace K {

	/** rotation only */
	TEST(ICP, calcRot) {

		// generate starting points
		std::vector<Point3f> set1;
		for (int i = 0; i < 9; ++i) {
			const float x = (float) (rand() % 64);
			const float y = (float) (rand() % 64);
			const float z = (float) (rand() % 64);
			set1.push_back(Point3f(x, y, z));
		}

		// rotate them using a well-known rotation matrix
		Mat4f mat = MatrixHelper::getRotation(Vec3f(1,2,3));
		std::vector<Point3f> set2;
		for (Point3f p1 : set1) {
			const Point3f p2 = mat * p1;
			set2.push_back(p2);
		}

		// estimate the rotation matrix using ICP
		Mat4f imat = ICP::solve(set1, set2);

		// rotate the initial point-set using the estimated icp matrix...
		std::vector<Point3f> set3;
		for (Point3f p1 : set1) {
			const Point3f p2 = imat * p1;
			set3.push_back(p2);
		}

		// ... orig-ROT*p must be near ICP-rot*p
		for (int i = 0; i < (int) set1.size(); ++i) {
			ASSERT_NEAR(0, set2[i].getDistance(set3[i]), 0.01);
		}

	}

	/** translation only */
	TEST(ICP, calcTra) {

		// generate starting points
		std::vector<Point3f> set1;
		for (int i = 0; i < 9; ++i) {
			const float x = (float) (rand() % 64);
			const float y = (float) (rand() % 64);
			const float z = (float) (rand() % 64);
			set1.push_back(Point3f(x, y, z));
		}

		// translate them using a well-known translation matrix
		Mat4f mat = MatrixHelper::getTranslation(Vec3f(3,5,7));
		std::vector<Point3f> set2;
		for (Point3f p1 : set1) {
			const Point3f p2 = mat * p1;
			set2.push_back(p2);
		}

		// estimate the translation matrix using ICP
		Mat4f imat = ICP::solve(set1, set2);

		// rotate the initial point-set using the estimated icp matrix...
		std::vector<Point3f> set3;
		for (Point3f p1 : set1) {
			const Point3f p2 = imat * p1;
			set3.push_back(p2);
		}

		// ... orig-TRA*p must be near ICP-TRA*p
		for (int i = 0; i < (int) set1.size(); ++i) {
			ASSERT_NEAR(0, set2[i].getDistance(set3[i]), 0.01);
		}

	}


	/** translation, rotation */
	TEST(ICP, calcTraRot) {

		// generate starting points
		std::vector<Point3f> set1;
		for (int i = 0; i < 9; ++i) {
			const float x = (float) (rand() % 64);
			const float y = (float) (rand() % 64);
			const float z = (float) (rand() % 64);
			set1.push_back(Point3f(x, y, z));
		}

		// translate them using a well-known translation matrix
		Mat4f mat = MatrixHelper::getTranslation(Vec3f(3,5,7)) * MatrixHelper::getRotation(Vec3f(1,2,3)) * MatrixHelper::getTranslation(Vec3f(3,1,2));
		std::vector<Point3f> set2;
		for (Point3f p1 : set1) {
			const Point3f p2 = mat * p1;
			set2.push_back(p2);
		}

		// estimate the translation matrix using ICP
		Mat4f imat = ICP::solve(set1, set2);

		// rotate the initial point-set using the estimated icp matrix...
		std::vector<Point3f> set3;
		for (Point3f p1 : set1) {
			const Point3f p2 = imat * p1;
			set3.push_back(p2);
		}

		// ... orig-TRA*p must be near ICP-TRA*p
		for (int i = 0; i < (int) set1.size(); ++i) {
			ASSERT_NEAR(0, set2[i].getDistance(set3[i]), 0.01);
		}

		// test inverse rotation [backt to set 1]
		Mat4f imat_inverse = imat.inverse();
		std::vector<Point3f> set4;
		for (Point3f p1 : set2) {
			const Point3f p2 = imat_inverse * p1;
			set4.push_back(p2);
		}

		// check set4 against set1
		for (int i = 0; i < (int) set1.size(); ++i) {
			ASSERT_NEAR(0, set4[i].getDistance(set1[i]), 0.01);
		}

		int i = 0; (void) i;

	}


}

#endif
