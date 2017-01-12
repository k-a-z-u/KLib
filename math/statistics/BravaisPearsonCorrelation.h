#ifndef K_BRAVAISPEARSONCORRELATION_H
#define K_BRAVAISPEARSONCORRELATION_H

#include <vector>
#include "../../Assertions.h"

// https://de.mathworks.com/help/symbolic/mupad_ref/stats-correlation.html

namespace K {

	class BravaisPearsonCorrelation {

	public:



		template <typename Scalar> static float get(const std::vector<Scalar>& a, const std::vector<Scalar>& b) {

			_assertEqual(a.size(), b.size(), "input size mismatch!");

			return get(a.data(), b.data(), a.size());

		}

		template <typename Scalar> static float get(const Scalar* a, const Scalar* b, const size_t numValues) {

			const float avgA = avg(a, numValues);
			const float avgB = avg(b, numValues);

			float sumA2 = 0;
			float sumB2 = 0;
			float sumAB = 0;

			for (size_t i = 0; i < numValues; ++i) {
				const float va = a[i];
				const float vb = b[i];

				const float da = va-avgA;
				const float db = vb-avgB;

				sumAB += da*db;
				//sumAB += va*db - avgA*db;		// same as above: https://de.mathworks.com/help/symbolic/mupad_ref/stats-correlation.html
				sumA2 += da*da;
				sumB2 += db*db;
			}

			return (sumAB) / std::sqrt(sumA2*sumB2);

		}



	private:

		/** get the average of all values within v*/
		template <typename Scalar> static float avg(const Scalar* v, const size_t numValues) {
			float sum = 0;
			for (size_t i = 0; i < numValues; ++i) {sum += v[i];}
			return sum / (float)numValues;
		}


//		/** get the average of all values within v*/
//		template <typename Scalar> static float avg(const std::vector<Scalar>& v) {
//			float sum = 0;
//			for (const Scalar s : v) {sum += s;}
//			return sum / (float)v.size();
//		}

	};

}

#endif // K_BRAVAISPEARSONCORRELATION_H
