
#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../math/vector/Vector.h"

namespace K {

	/** test initialization */
	TEST(Vector, init) {

		Vector<double, 2> vec2 = {1,2};
		ASSERT_EQ(vec2[0], 1);
		ASSERT_EQ(vec2[1], 2);

		Vector<double, 3> vec3 = {1,2,3};
		ASSERT_EQ(vec3[0], 1);
		ASSERT_EQ(vec3[1], 2);
		ASSERT_EQ(vec3[2], 3);

		Vector<double, 4> vec4 = {1,2,3,4};
		ASSERT_EQ(vec4[0], 1);
		ASSERT_EQ(vec4[1], 2);
		ASSERT_EQ(vec4[2], 3);
		ASSERT_EQ(vec4[3], 4);

		Vector<double, 4> vec4b;
		ASSERT_EQ(vec4b[0], 0);
		ASSERT_EQ(vec4b[1], 0);
		ASSERT_EQ(vec4b[2], 0);
		ASSERT_EQ(vec4b[3], 0);

	}

	TEST(Vector, initFromArray) {

		double arr1[] = {1,2,3};
		Vector<double, 3> vec1(arr1);
		ASSERT_EQ(vec1[0], arr1[0]);
		ASSERT_EQ(vec1[1], arr1[1]);
		ASSERT_EQ(vec1[2], arr1[2]);

		float arr2[] = {4,2,1,8};
		Vector<float, 4> vec2(arr2);
		ASSERT_EQ(vec2[0], arr2[0]);
		ASSERT_EQ(vec2[1], arr2[1]);
		ASSERT_EQ(vec2[2], arr2[2]);
		ASSERT_EQ(vec2[3], arr2[3]);

	}

	TEST(Vector, add) {

		Vector<double, 3> vec1 = {1,2,3};
		Vector<double, 3> vec2 = {4,5,6};
		Vector<double, 3> vec3 = {7,8,9};
		Vector<double, 3> vecR = vec1 + vec2 + vec3;
		ASSERT_EQ(vecR[0], 12);
		ASSERT_EQ(vecR[1], 15);
		ASSERT_EQ(vecR[2], 18);

		vecR = vec1 += vec2;
		ASSERT_EQ(vec1[0], 5);
		ASSERT_EQ(vec1[1], 7);
		ASSERT_EQ(vec1[2], 9);

		ASSERT_EQ(vecR[0], 5);
		ASSERT_EQ(vecR[1], 7);
		ASSERT_EQ(vecR[2], 9);

		ASSERT_EQ(vec1, vecR);

	}

	TEST(Vector, sub) {

		Vector<double, 3> vec1 = {1,2,3};
		Vector<double, 3> vec2 = {4,5,6};
		Vector<double, 3> vec3 = {7,8,9};
		Vector<double, 3> vecR = vec1 - vec2 - vec3;
		ASSERT_EQ(vecR[0], -10);
		ASSERT_EQ(vecR[1], -11);
		ASSERT_EQ(vecR[2], -12);

		vecR = vec1 -= vec2;
		ASSERT_EQ(vec1[0], -3);
		ASSERT_EQ(vec1[1], -3);
		ASSERT_EQ(vec1[2], -3);

		ASSERT_EQ(vecR[0], -3);
		ASSERT_EQ(vecR[1], -3);
		ASSERT_EQ(vecR[2], -3);

		ASSERT_EQ(vec1, vecR);

	}

	TEST(Vector, mul) {

		Vector<double, 3> vec1 = {1,2,3};
		Vector<double, 3> vec2 = {4,5,6};
		Vector<double, 3> vec3 = {7,8,9};
		Vector<double, 3> vecR = vec1 * vec2 * vec3;
		ASSERT_EQ(vecR[0], 28);
		ASSERT_EQ(vecR[1], 80);
		ASSERT_EQ(vecR[2], 162);

		vecR = vec1 *= vec2;
		ASSERT_EQ(vec1[0], 4);
		ASSERT_EQ(vec1[1], 10);
		ASSERT_EQ(vec1[2], 18);

		ASSERT_EQ(vecR[0], 4);
		ASSERT_EQ(vecR[1], 10);
		ASSERT_EQ(vecR[2], 18);

		ASSERT_EQ(vec1, vecR);

	}

	TEST(Vector, div) {

		Vector<double, 3> vec1 = {6,18,21};
		Vector<double, 3> vec2 = {3,2,7};
		Vector<double, 3> vec3 = {2,3,2};
		Vector<double, 3> vecR = vec1 / vec2 / vec3;
		ASSERT_EQ(vecR[0], 1);
		ASSERT_EQ(vecR[1], 3);
		ASSERT_EQ(vecR[2], 1.5);

		vecR = vec1 /= vec2;
		ASSERT_EQ(vec1[0], 2);
		ASSERT_EQ(vec1[1], 9);
		ASSERT_EQ(vec1[2], 3);

		ASSERT_EQ(vecR[0], 2);
		ASSERT_EQ(vecR[1], 9);
		ASSERT_EQ(vecR[2], 3);

		ASSERT_EQ(vec1, vecR);

	}

	TEST(Vector, misc) {

		Vector<double, 3> vec1 = {6,18,21};
		Vector<double, 3> vec2 = {3,2,7};
		Vector<double, 3> vec3 = {2,3,2};

		Vector<double, 3> vecR = vec1 / vec2 + vec3 * vec3 - vec2;

		ASSERT_EQ(vecR[0], 3);
		ASSERT_EQ(vecR[1], 16);
		ASSERT_EQ(vecR[2], 0);

	}

	TEST(Vector, orthogonal) {

		Vector<double, 2> vec1 = {6,18};
		Vector<double, 2> vecR = vec1.getOrthogonal();

		ASSERT_EQ(vecR[0], -18);
		ASSERT_EQ(vecR[1], 6);

	}

	TEST(Vector, length) {

		Vector<double, 2> vec2 = {6,7};
		ASSERT_FLOAT_EQ(9.219544f, (float) vec2.getLength());

		Vector<double, 3> vec3 = {1,2,3};
		ASSERT_FLOAT_EQ(3.7416575f, (float) vec3.getLength());

	}

}

#endif
