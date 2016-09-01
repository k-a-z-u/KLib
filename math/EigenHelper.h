#ifndef EIGENHELPER_H
#define EIGENHELPER_H

#include "../geo/Point3.h"

namespace K {

	/**
	 * helper-methods for libEigen
	 */
	class EigenHelper {

	public:

		/** get the index of the smallest element within the given vector */
		template <typename Vec> static int getMinIndex(const Vec vec) {
			int minIdx = -1;
			float minVal = INFINITY;
			for (int i = 0; i < vec.rows(); ++i) {
				if (vec(i) < minVal) {minVal = vec(i); minIdx = i;}
			}
			return minIdx;
		}

		/** get the eigenvector for the samllest eigenvalue */
		template <typename Mat, typename Vec> static Vec getMinEigenVector(const Mat evec, const Vec eval) {
			const int minIdx = getMinIndex(eval);
			return evec.col(minIdx);
		}





	};

}

#endif // EIGENHELPER_H
