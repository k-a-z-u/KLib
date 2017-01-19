#ifndef K_MATH_DISTRIBUTION_NORMAL_N_H
#define K_MATH_DISTRIBUTION_NORMAL_N_H


#include <eigen3/Eigen/Dense>

namespace K {

	class NormalDistributionN {

	private:

		const Eigen::VectorXd mu;
		const Eigen::MatrixXd sigma;

		const double _a;
		const Eigen::MatrixXd _sigmaInv;

	public:

		/** ctor */
		NormalDistributionN(const Eigen::VectorXd mu, const Eigen::MatrixXd sigma) :
			mu(mu), sigma(sigma), _a( 1.0 / std::sqrt( (sigma * 2.0 * M_PI).determinant() ) ), _sigmaInv(sigma.inverse()) {

		}


		/** get probability for the given value */
		double getProbability(const Eigen::VectorXd val) const {
			const double b = ((val-mu).transpose() * _sigmaInv * (val-mu));
			return _a * std::exp(-b/2.0);
		}

	};

}

#endif // K_MATH_DISTRIBUTION_NORMAL_H
