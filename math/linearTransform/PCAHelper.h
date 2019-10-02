#ifndef K_MATH_PATTERN_PCA_PCAHELPER_H
#define K_MATH_PATTERN_PCA_PCAHELPER_H

#include <eigen3/Eigen/Dense>
#include <vector>

namespace K {

	using namespace Eigen;

	#define unlikely(x)	__builtin_expect(!!(x), 0)


	template <typename ScalarType> using DynColVector = Matrix<ScalarType, Dynamic, 1>;
	template <typename ScalarType> using DynMatrix = Matrix<ScalarType, Dynamic, Dynamic>;

	template <typename ScalarType> class PCAHelper {

	public:

		/** convert an std::vector<T> to an Eigen::Vector<T> */
		static DynColVector<ScalarType> toVector(const std::vector<ScalarType>& vec) {
			return Map<DynColVector<ScalarType>>((ScalarType*)vec.data(), vec.size());
		}

		/** use the column-vector as-is */
		static DynColVector<ScalarType> toVector(const DynColVector<ScalarType>& vec) {
			return vec;
		}

		/** calculate the R-matrix for several added pattern-vectors */
		class R {

		private:

			int cnt;
			DynMatrix<ScalarType> _R;

		public:

			/** ctor */
			R() : cnt(0) {;}

			/** add the next pattern-vector to the R-matrix */
			template <typename T> void add(const T& val) {
				const DynColVector<ScalarType> vec = toVector(val);
				if (unlikely(cnt == 0)) {
					_R = vec * vec.transpose();		// first call: initialize R
				} else {
					_R += vec * vec.transpose();	// subsequent calls: update R
				}
				++cnt;
			}

			/** get the calculated R-matrix */
			DynMatrix<ScalarType> get() {return _R / (ScalarType)cnt;}

		};


		/** calculate the m-vector for the several added pattern-vectors */
		class m {

		private:

			int cnt;
			DynColVector<ScalarType> _m;

		public:

			/** ctor */
			m() : cnt(0) {;}

			/** add the next pattern-vector to the m-vector */
			template <typename T> void add(const T& val) {
				const DynColVector<ScalarType> vec = toVector(val);
				if (unlikely(cnt == 0)) {
					_m = vec;						// first call: initialize m
				} else {
					_m += vec;						// subsequent calls: update m
				}
				++cnt;
			}

			/** get the calculated m-vector */
			DynMatrix<ScalarType> get() {return _m / (ScalarType)cnt;}

		};

		/** calculate the R-matrix for the given (iterable) input vectors */
		template <typename T> static DynMatrix<ScalarType> getR(const T& vecs) {
			R calc;
			for (const auto& vec : vecs) {calc.add(vec);}
			return calc.get();
		}

		/** calculate the m-vector for the given (iterable) input vectors */
		template <typename T> static DynMatrix<ScalarType> getM(const T& vecs) {
			m calc;
			for (const auto& vec : vecs) {calc.add(vec);}
			return calc.get();
		}



	};

}

#endif // K_MATH_PATTERN_PCA_PCAHELPER_H

