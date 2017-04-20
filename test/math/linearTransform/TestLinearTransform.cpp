#ifdef WITH_TESTS

#include "../../Test.h"
#include "../../../math/linearTransform/PCA.h"

namespace K {

	TEST(LinearTransform, getM) {

		std::vector<float> vec1 = {1, 2, 3};
		std::vector<float> vec2 = {2, 3, 4};
		std::vector<float> vec3 = {3, 4, 5};
		std::vector<std::vector<float>> vecs = {vec1,vec2,vec3};

		DynColVector<float> m = PCAHelper<float>::getM(vecs);
		DynColVector<float> mRes(3); mRes << 2, 3, 4;

		ASSERT_EQ(mRes, m);

	}

	TEST(LinearTransform, getR) {

		std::vector<float> vec1 = {0, 3, 6};
		std::vector<float> vec2 = {3, 6, 9};
		std::vector<float> vec3 = {6, 9, 12};
		std::vector<std::vector<float>> vecs = {vec1,vec2,vec3};

		DynMatrix<float> R = PCAHelper<float>::getR(vecs);
		DynMatrix<float> RRes(3,3); RRes <<
				15, 24, 33,
				24, 42, 60,
				33, 60, 87;

		//std::cout << R << std::endl;
		//std::cout << RRes << std::endl;

		ASSERT_EQ(RRes, R);

	}

	TEST(LinearTransform, PCA_getQ) {

		std::vector<float> vec1 = {0, 3, 6};
		std::vector<float> vec2 = {3, 6, 9};
		std::vector<float> vec3 = {6, 9, 12};

		LinearTransform<float>::PCA pca;
		pca.add(vec1); pca.add(vec2); pca.add(vec3);

		DynMatrix<float> Q = pca.getQ();
		DynMatrix<float> QRes(3,3); QRes <<
				6, 6, 6,
				6, 6, 6,
				6, 6, 6;

		ASSERT_EQ(QRes, Q);

	}

	/** compare two matrices */
	void assertEq(DynMatrix<float> a, DynMatrix<float> b) {
		for (int r = 0; r < a.rows(); ++r) {
			for (int c = 0; c < a.cols(); ++c) {
				ASSERT_NEAR(a(r,c), b(r,c), 0.0001);
			}
		}
	}

	TEST(LinearTransform, PCA_getA) {

		std::vector<float> vec1 = {1, 2, 3};
		std::vector<float> vec2 = {4, 5, 6};
		std::vector<float> vec3 = {9, 8, 7};

		LinearTransform<float>::PCA pca;
		pca.add(vec1); pca.add(vec2); pca.add(vec3);

		DynMatrix<float> A = pca.getA(2);
		DynMatrix<float> ARes(2,3); ARes <<
				0.745924f, 0.556174f, 0.366425f,
				-0.52624f, 0.154927f, 0.836104f;

		assertEq(ARes, A);

	}


	/** ensure the calculated kernel matrices are correct */
	TEST(LinearTransform, getQ) {

		std::vector<float> c1v1 = {1,2,3,4};
		std::vector<float> c1v2 = {1,2,4,5};

		std::vector<float> c2v1 = {3,8,9,0};
		std::vector<float> c2v2 = {3,7,5,0};

		std::vector<float> c3v1 = {9,1,3,2};
		std::vector<float> c3v2 = {7,3,3,2};

		LinearTransform<float>::PCA pca;
		pca.add(c1v1); pca.add(c1v2);
		pca.add(c2v1); pca.add(c2v2);
		pca.add(c3v1); pca.add(c3v2);

		DynMatrix<float> Q1 = pca.getQ();
		DynMatrix<float> Q1Res(4,4); Q1Res <<
			9.0000f,  -2.1667f,  -1.8333f,  -1.8333f,
			-2.1667f,   7.1389f,   4.9167f,  -3.9722f,
			-1.8333f,   4.9167f,   4.5833f,  -2.4167f,
			-1.8333f,  -3.9722f,  -2.4167f,   3.4722f;
		assertEq(Q1Res, Q1);

		LinearTransform<float>::MaxInterClassDistance<int> inter;
		inter.add(1, c1v1); inter.add(1,c1v2);
		inter.add(2, c2v1); inter.add(2,c2v2);
		inter.add(3, c3v1); inter.add(3,c3v2);

		DynMatrix<float> Q2 = inter.getQ();
		DynMatrix<float> Q2Res(4,4); Q2Res <<
			13.3333f,   -3.0833f,   -2.7500f,   -2.7500f,
			-3.0833f,   10.5000f,    7.2083f,   -5.9583f,
			-2.7500f,    7.2083f,    6.1667f,   -3.6667f,
			-2.7500f,   -5.9583f,   -3.6667f,    5.1667f;
		assertEq(Q2Res, Q2);

		LinearTransform<float>::MinIntraClassDistance<int> intra;
		intra.add(1, c1v1); intra.add(1,c1v2);
		intra.add(2, c2v1); intra.add(2,c2v2);
		intra.add(3, c3v1); intra.add(3,c3v2);

		DynMatrix<float> Q3 = intra.getQ();
		DynMatrix<float> Q3Res(4,4); Q3Res <<
			0.33333f,  -0.33333f,   0.00000f,   0.00000f,
			-0.33333f,   0.41667f,   0.33333f,   0.00000f,
			 0.00000f,   0.33333f,   1.41667f,   0.08333f,
			 0.00000f,   0.00000f,   0.08333f,   0.08333f;
		assertEq(Q3Res, Q3);


	}



}

#endif
