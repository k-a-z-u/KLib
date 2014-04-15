

#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../math/vector/Matrix.h"

namespace K {

	TEST(Matrix, clear) {

		Matrix<float, 10, 10> mat;

		mat.clear();
		for (unsigned int y = 0; y < 10; ++y) {
			for (unsigned int x = 0; x < 10; ++x) {
				EXPECT_EQ(0, mat.get(x,y));
			}
		}

	}

	TEST(Matrix, identity) {
		Matrix<float, 10, 10> mat;
		mat.identity();
		for (unsigned int y = 0; y < 10; ++y) {
			for (unsigned int x = 0; x < 10; ++x) {
				EXPECT_EQ((x==y) ? 1 : 0, mat.get(x,y));
			}
		}
	}

	TEST(Matrix, vecMul) {

		Matrix<float, 3, 3>mat;
		mat.identity();

		Vector<float, 3> vec = {1,2,3};

		Vector<float, 3> vec2 = mat * vec;
		EXPECT_EQ(vec2, vec);

		mat.clear();
		vec2 = mat*vec;
		EXPECT_EQ(0, vec2[0]);
		EXPECT_EQ(0, vec2[1]);
		EXPECT_EQ(0, vec2[2]);

		mat.set(0,0, 1);
		mat.set(1,0, 2);
		mat.set(0,1, 3);
		mat.set(1,1, 4);
		std::cout << mat << std::endl;
		vec2 = mat*vec;
		EXPECT_EQ(5, vec2[0]);
		EXPECT_EQ(11, vec2[1]);
		EXPECT_EQ(0, vec2[2]);

	}



}

#endif
