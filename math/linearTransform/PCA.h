#ifndef K_MATH_PATTERN_PCA_PCA_H
#define K_MATH_PATTERN_PCA_PCA_H

#include "PCAHelper.h"
#include <vector>
#include <unordered_map>

namespace K {

	/**
	 * the subclasses within this class provide linear-transformation based
	 * on several optimization criterias.
	 */
	template <typename ScalarType> class LinearTransform {

	private:

		/** attributes we want to calculate per-class */
		struct ClassParams {
			typename PCAHelper<ScalarType>::R calcR;
			typename PCAHelper<ScalarType>::m calcM;
		};

	public:

		/**
		 * PCA: maximize the distance between all patterns (no matter what class they belong to)
		 */
		class PCA {

		private:

			typename PCAHelper<ScalarType>::R calcR;
			typename PCAHelper<ScalarType>::m calcM;

		public:

			/** add one new pattern-vector to the PCA calculation */
			template <typename T> void add(const T& patternVector) {
				calcR.add(patternVector);
				calcM.add(patternVector);
			}



			/** get the kernel-matrix Q */
			DynMatrix<ScalarType> getQ() {
				const DynMatrix<ScalarType> R = calcR.get();
				const DynColVector<ScalarType> m = calcM.get();
				return R - m * m.transpose();
			}

			/** get the transformation-matrix A */
			DynMatrix<ScalarType> getA(const int num) {

				// calculate (sorted) eigenvectors from the Q-matrix
				std::vector<std::pair<ScalarType, DynColVector<ScalarType>>> ev = getSortedEigenvectors(getQ(), true);

				// build transformation A;
				return getMatrix(ev, num);

			}

		};


		template <typename ClassType> class Base {

		public:

			/** calculated parameters per class */
			std::vector<ClassParams> classes;

			/** mapp betwenn class-type and an index */
			std::unordered_map<ClassType, int> classToIndex;

		public:

			/** add one new (classified) pattern-vector to the calculation */
			template <typename T> void add(const ClassType& clazz, const T& patternVector) {

				// first pattern for this class?
				if (classToIndex.find(clazz) == classToIndex.end()) {
					classToIndex[clazz] = (int) classes.size();
					classes.push_back(ClassParams());
				}

				// add pattern values
				const int idx = classToIndex[clazz];
				classes[idx].calcR.add(patternVector);
				classes[idx].calcM.add(patternVector);

			}



		};

		/**
		 *  maximize the distance between all patterns of all provided classes
		 */
		template <typename ClassType> class MaxInterClassDistance : public Base<ClassType> {

		public:

			 /** get the kernel-matrix Q */
			 DynMatrix<ScalarType> getQ() {

				 // allocate
				 const int classes = (int) this->classes.size();
				 const size_t size = this->classes[0].calcM.get().rows();
				 DynMatrix<ScalarType> mSum(size, size); mSum.setZero();
				 DynMatrix<ScalarType> rSum(size, size); rSum.setZero();

				 // calculate R and normalize it
				 for (size_t k = 0; k < this->classes.size(); ++k) {
					 rSum += this->classes[k].calcR.get();
				 }
				 rSum /= (ScalarType) classes;

				 // calculate m and normalize it
				 for (size_t k = 1; k < this->classes.size(); ++k) {
					 for (size_t l = 0; l <= k-1; ++l) {
						 const DynColVector<ScalarType> mK = this->classes[k].calcM.get();
						 const DynColVector<ScalarType> mL = this->classes[l].calcM.get();
						 mSum += mK * mL.transpose() + mL * mK.transpose();
					 }
				 }
				 mSum /= (ScalarType) (classes * (classes - 1));

				 // done
				 return rSum - mSum;

			 }

			 /** get the transformation-matrix A */
			 DynMatrix<ScalarType> getA(const int num) {

				 // calculate (sorted) eigenvectors from the Q-matrix
				 std::vector<std::pair<ScalarType, DynColVector<ScalarType>>> ev = getSortedEigenvectors(getQ(), true);

				 // build transformation A;
				 return getMatrix(ev, num);

			 }

		};

		/**
		 *  minimize the distance of all patterns within one class
		 */
		template <typename ClassType> class MinIntraClassDistance : public Base<ClassType> {

		public:

			 /** get the kernel-matrix Q */
			 DynMatrix<ScalarType> getQ() {

				 // allocate
				 const size_t size = this->classes[0].calcM.get().rows();
				 DynMatrix<ScalarType> Q(size, size); Q.setZero();

				 // calculate Q and normalize it
				 for (size_t k = 0; k < this->classes.size(); ++k) {

					 const DynMatrix<ScalarType> R = this->classes[k].calcR.get();
					 const DynColVector<ScalarType> m = this->classes[k].calcM.get();
					Q += R - m*m.transpose();

				 }
				 Q /= (ScalarType) this->classes.size();

				 return Q;

			 }

			 /** get the transformation-matrix A */
			 DynMatrix<ScalarType> getA(const int num) {

				 // calculate (sorted) eigenvectors from the Q-matrix
				 std::vector<std::pair<ScalarType, DynColVector<ScalarType>>> ev = getSortedEigenvectors(getQ(), false);

				 // build transformation A;
				 return getMatrix(ev, num);

			 }

		};





		/** get the eigenvalues for the given matrix */
		static std::vector<std::pair<ScalarType, DynColVector<ScalarType>>> getSortedEigenvectors(const DynMatrix<ScalarType>& mat, const bool desc) {

			// calculate eigenvalues for the matrix
			SelfAdjointEigenSolver<DynMatrix<ScalarType>> es(mat);

			// group eigenvalues with their eigenvectors
			std::vector<std::pair<ScalarType, DynColVector<ScalarType>>> pairs;
			for (int i = 0; i < es.eigenvalues().rows(); ++i) {
				const ScalarType eVal = es.eigenvalues()(i);
				const DynColVector<ScalarType> eVec = es.eigenvectors().col(i);
				pairs.push_back(std::make_pair(eVal, eVec));
			}

			// sort them by eigenvalues (largest first)
			auto lDesc = [] (const std::pair<ScalarType, DynColVector<ScalarType>>& p1, const std::pair<ScalarType, DynColVector<ScalarType>> p2) {return p1.first > p2.first;};
			// sort them by eigenvalues (smallest first)
			auto lAsc = [] (const std::pair<ScalarType, DynColVector<ScalarType>>& p1, const std::pair<ScalarType, DynColVector<ScalarType>> p2) {return p1.first < p2.first;};

			// sort
			std::sort(pairs.begin(), pairs.end(), (desc) ? (lDesc) : (lAsc));

			// done
			return pairs;

		}

		static DynMatrix<ScalarType> getMatrix(std::vector<std::pair<ScalarType, DynColVector<ScalarType>>>& sortedEigenvectors, const int num) {

			// allocate output matrix
			DynMatrix<ScalarType> mat(sortedEigenvectors[0].second.rows(), num);

			// insert eigenvectors
			for (int i = 0; i < num; ++i) {
				mat.col(i) = sortedEigenvectors[i].second;
			}

			// done
			return mat.transpose();

		}



	};

}

#endif // K_MATH_PATTERN_PCA_PCA_H

