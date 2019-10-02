#ifndef K_MATH_DISTRIBUTION_NORMALNFACTORY_H
#define K_MATH_DISTRIBUTION_NORMALNFACTORY_H

#include <eigen3/Eigen/Dense>
#include <vector>
#include "NormalN.h"
#include "../../Assertions.h"

namespace K {

	/** factory class to build normal distribution from samples */
	template <typename Scalar> class NormalNFactory {

	private:

		using Vec = Eigen::Matrix<Scalar, Eigen::Dynamic, 1>;
		using Mat = Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic>;

		bool init = false;
		int cnt = 0;

		/** sum of all added values */
		Vec sum;

		/** squared sum of all added values */
		Mat sum2;


		/** add a new sample vector to the distribution */
		void _add(const Vec& f) {

			if (!init) {
				sum = f;
				sum2 = f * f.transpose();
				init = true;
			} else {
				sum += f;
				sum2 += f * f.transpose();
			}

			++cnt;

		}


	public:

		template <typename ScalarToAdd> void add(const Eigen::Matrix<ScalarToAdd, Eigen::Dynamic, 1>& vec) {
			_add(vec.template cast<Scalar>());
		}

		void add(const Eigen::Matrix<Scalar, Eigen::Dynamic, 1>& vec) {
			_add(vec);
		}


		/** get the covariance matrix */
		Mat getCovar() const {
			return (sum2/cnt) - ( (sum/cnt) * (sum/cnt).transpose() );
		}

		/** get the average vecot */
		Vec getAvg() const {
			return sum/cnt;
		}

		/** get a normal-distribution that describes all contained samples */
		K::NormalDistributionN<Scalar> getNormalDistribution() const {

			const auto avg = getAvg();
			const auto var = getCovar();
			for (Eigen::Index i = 0; i < avg.rows(); ++i) {
				_assertNotNAN(avg[i], "detected NaN");
			}
			for (Eigen::Index row = 0; row < avg.rows(); ++row) {
				for (Eigen::Index col = 0; col < avg.cols(); ++col) {
					_assertNotNAN(var(row,col), "detected NaN");
				}
			}

			return K::NormalDistributionN<Scalar> (avg, var);

		}


	};

}

#endif // K_MATH_DISTRIBUTION_NORMALNFACTORY_H
