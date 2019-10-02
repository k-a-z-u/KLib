#ifndef K_MATH_DISTRIBUTION_NORMAL_N_H
#define K_MATH_DISTRIBUTION_NORMAL_N_H

#include "../../Assertions.h"

#include <eigen3/Eigen/Dense>

namespace K {

	template <typename Scalar = double> class NormalDistributionN {

	public:

		using Vector = Eigen::Matrix<Scalar, Eigen::Dynamic, 1>;
		using Matrix = Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic>;

	private:

		/** average vector */
		const Vector mu;

		/** covariance matrix */
		const Matrix covar;

		/** pre-calculated internal values */
		const Scalar _a;
		const Matrix _covarInv;

	public:

		/** ctor */
		NormalDistributionN(const Vector& mu, const Matrix& covar) :
			mu(mu), covar(covar), _a( 1.0 / std::sqrt( (covar * 2.0 * M_PI).determinant() ) ), _covarInv(covar.inverse()) {
			//mu(mu), covar(covar), _a( 1.0 / std::sqrt( 2.0 * M_PI * covar.determinant() ) ), _covarInv(covar.inverse()) {

			// sanity check
			for (Eigen::Index row = 0; row < covar.rows(); ++row) {
				for (Eigen::Index col = 0; col < covar.cols(); ++col) {
					_assertNotNAN(covar(row,col), "detected NaN in covariance");
				}
			}

			for (Eigen::Index row = 0; row < _covarInv.rows(); ++row) {
				for (Eigen::Index col = 0; col < _covarInv.cols(); ++col) {
					_assertNotNAN(_covarInv(row,col), "detected NaN in inverse-covariance");
				}
			}

		}

		/** get probability for the given value */
		template<typename ScalarToAdd>
		Scalar getProbability(const Eigen::Matrix<ScalarToAdd, Eigen::Dynamic, 1>& val) const {
			return getProbability(val.template cast<Scalar>());
		}

		Scalar getProbability(const Vector& val) const {
			const double b = ((val-mu).transpose() * _covarInv * (val-mu));
			_assertNotNAN(b, "Detected NaN");
			return _a * std::exp(-b/2.0);
		}

	};

}

#endif // K_MATH_DISTRIBUTION_NORMAL_H
