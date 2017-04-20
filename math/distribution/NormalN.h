#ifndef K_MATH_DISTRIBUTION_NORMAL_N_H
#define K_MATH_DISTRIBUTION_NORMAL_N_H


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
		const double _a;
		const Matrix _covarInv;

	public:

		/** ctor */
		NormalDistributionN(const Vector& mu, const Matrix& covar) :
			mu(mu), covar(covar), _a( 1.0 / std::sqrt( (covar * 2.0 * M_PI).determinant() ) ), _covarInv(covar.inverse()) {
			//mu(mu), covar(covar), _a( 1.0 / std::sqrt( 2.0 * M_PI * covar.determinant() ) ), _covarInv(covar.inverse()) {
			;
		}

		/** get probability for the given value */
		double getProbability(const Vector& val) const {
			const double b = ((val-mu).transpose() * _covarInv * (val-mu));
			return _a * std::exp(-b/2.0);
		}

	};

}

#endif // K_MATH_DISTRIBUTION_NORMAL_H
