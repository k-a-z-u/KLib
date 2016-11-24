#ifndef ELLIPSEESTIMATORMATH_H
#define ELLIPSEESTIMATORMATH_H

#include <eigen3/Eigen/Dense>
#include "../../math/EigenHelper.h"

#include "../../geo/Ellipse.h"

// Notes
// http://stackoverflow.com/questions/21765015/unnormalization-of-ellipse-coefficients-after-direct-ellipse-fitting#30291904

namespace K {

	namespace EllipseEstimator {

		struct Estimation {

			// raw ellipse parameters A,B,C,D,E,F
			Eigen::Matrix<double, 6, 1> params;

//			// scaling (used for numerical stability)
//			float s;

			Estimation()  {
				params << 0,0,0,0,0,1;
			}

			//Estimation(const Eigen::Matrix<float, 6, 1>& params, const float s) : params(params), s(s) {;}

//			static Estimation fromSVD(const Eigen::Matrix<float, 6, 1>& vec) {
//				Estimation est;
//				est.params = vec;
//				return est;
//			}

			static Estimation fromSVD(const Eigen::Matrix<double, 6, 1>& vec) {
				Estimation est;
				est.params = vec;// << vec(0), vec(1), vec(2), vec(3), vec(4), vec(5);
				return est;
			}

//			static Estimation fromEV(const Eigen::Matrix<double, 6, 1>& vec, const float s) {
//				Estimation est;
//				//est.params << vec(0), 2*vec(1), vec(2), 2*s*vec(3), 2*s*vec(4), s*s*vec(5);		// book
//				est.params << vec(0), vec(1), vec(2), s*vec(3), s*vec(4), s*s*vec(5);				// non-book
//				est.params /= est.params.norm();
//				if (est.params(5) < 0) {est.params = -est.params;}
//				return est;
//			}

			/** @see getParams(); */
			Ellipse::CanonicalParams toEllipse() const {

				Eigen::Matrix<double, 6, 1> vec = params;

//				// [re-adjust the scaling we added initially
//				vec << vec(0), 2*vec(1), vec(2), 2*s*vec(3), 2*s*vec(4), s*s*vec(5);									// Eq 2.2

//				// ensure vec is unit-length [this does NOT change the ellipse!]
//				vec /= vec.norm();

//				// ensure the F component is positive (otherwise negate the whole vector)
//				if (vec(5) < 0) {vec = -vec;}

				// construct cannonical parameters
				return Ellipse::CanonicalParams(vec(0), vec(1), vec(2), vec(3), vec(4), vec(5));

			}

//			float getErrorSampson(const float x, const float y) const {

//				//const float s = 100;
//				Eigen::Matrix<float, 6, 1> xi; xi << x*x, 2*x*y, y*y, 2*s*x, 2*s*y, s*s*1;

//				Eigen::Matrix<float, 6, 1> theta = params;

//				Eigen::Matrix<float, 6, 6> covar; covar <<
//					x*x,	x*y,	0,		x,		0,		0,
//					x*y,	x*x+y*y,x*y,	y,		x,		0,
//					0,		x*y,	y*y,	0,		y,		0,
//					x,		y,		0,		1,		0,		0,
//					0,		x,		y,		0,		1,		0,
//					0,		0,		0,		0,		0,		0;

//				const float a = xi.dot(theta);				// same as "getError()"
//				const float b = theta.dot(4*covar*theta);
//				return (a*a) / b;

//			}


			template <typename Scalar> static Ellipse::CanonicalParams get(const std::vector<Point2<Scalar>>& points) {
				Estimation est = getParams<Scalar>(points);
				return est.toEllipse();
			}


			/**
			 * estimate the ellipse for the given point-set using SVD.
			 * a point belongs to an ellipse identified by A,B,C,D,E,F if:
			 * Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0
			 * for this point. thus we add one of those equations per given point.
			 * the SVD estimates those A,B,C,D,E,F with the lowest overall error among all points.
			 */
			template <typename Scalar, typename List> static Estimation getParams(const List& points) {

				// number of points to use
				const size_t num = points.size();

				// matrix with one row (equation) per point
				Eigen::Matrix<double, Eigen::Dynamic, 6> mat;

				// we need points.size() rows
				mat.conservativeResize(num, Eigen::NoChange);

				// append one row (one equation) per point
				int row = 0;
				for (const Point2<Scalar>& p : points) {
					const double x = (double) p.x;
					const double y = (double) p.y;
					mat.row(row) << x*x,	x*y,	y*y,	x,		y,		1;			// Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0
					++row;
				}

				// calculate SVD with FullV (U is NOT needed but compilation fails when using thinU)
				const Eigen::JacobiSVD<decltype(mat)> svd(mat, Eigen::ComputeFullU | Eigen::ComputeFullV);
				const Eigen::JacobiSVD<decltype(mat)>::MatrixVType V = svd.matrixV();
				//const Eigen::JacobiSVD<decltype(mat)>::SingularValuesType D = svd.singularValues();

				// the 6 canonical parameters [A-F] are given by the vector in V corresponding to the smallest singular value
				// the smallest singular value belongs to the last index in U: thus "5"
				const Eigen::Matrix<double,6,1> vec = V.col(5);

				return Estimation::fromSVD(vec);


			}


			/**
			 * normalizes the given input points [zero mean, std-dev = 1]
			 * calculates generalized eigenvalues given 2 matrices [additional constraint]
			 * un-normalizes the eigenvector corresponding to the most-negative eigenvalue
			 */
			template <typename Scalar, typename List> static Estimation getParamsNormalized(const List& points) {

				using Element = K::Point2<Scalar>;

				Point2f sum;
				Point2f sum2;
				for (const Element p : points) {
					sum += p;
					sum2 += p*p;
				}

				// calculate average and std-dev for normalization
				const K::Point2f avg = sum / (float) points.size();
				const K::Point2f avg2 = sum2 / (float) points.size();
				//const K::Point2f stddev = K::Point2f(2.5, 4.5);//sqrt(avg2 - avg*avg);		// TODO just for testing
				const K::Point2f stddev = sqrt(avg2 - avg*avg);

				// straight line? -> done
				if (stddev.x == 0 || stddev.y == 0) {return Estimation();}

				// number of points to use
				const size_t num = points.size();

				// matrix with one row (equation) per point
				Eigen::Matrix<double, Eigen::Dynamic, 6> D;

				// we need points.size() rows
				D.conservativeResize(num, Eigen::NoChange);

				// append one row (one equation) per point
				int row = 0;
				for (const Element& _p : points) {
					const Point2f p = (Point2f((float)_p.x, (float)_p.y) - avg) / stddev;		// center around (0,0) with a range of apx. +/- 1
					const float x = (float) p.x;
					const float y = (float) p.y;
					D.row(row) << x*x,	x*y,	y*y,	x,		y,		1;			// Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0
					++row;
				}

				//std::cout << D << std::endl << std::endl;

				auto S = D.transpose() * D;
				//std::cout << S << std::endl << std::endl;

				Eigen::Matrix<double, 6, 6> C;
				C <<	0,	0,	-2,	0,	0,	0,
						0,	1,	0,	0,	0,	0,
						-2,	0,	0,	0,	0,	0,
						0,	0,	0,	0,	0,	0,
						0,	0,	0,	0,	0,	0,
						0,	0,	0,	0,	0,	0;

				//std::cout << C << std::endl << std::endl;

				// http://stackoverflow.com/questions/12672408/generalized-eigenvectors-in-matlab
				Eigen::GeneralizedEigenSolver<Eigen::Matrix<double, 6, 6>> eigs(S, C, true);
				//std::cout << eigs.eigenvalues() << std::endl;
				//std::cout << eigs.eigenvectors() << std::endl;

				int match = -1;
				for (int i = 0; i < 6; ++i) {
					const double ev = eigs.eigenvalues()(i).real();
					if (ev < 1e-8 && ev != -INFINITY) {
						match = i;
						break;
					}
				}

				// probably invalid ellipse data...
				if (match == -1) {return Estimation();}

				//std::cout << "match: " << match << std::endl;



				Eigen::Matrix<double,6,1> A;
					A <<
						eigs.eigenvectors().col(match).row(0).real(),
						eigs.eigenvectors().col(match).row(1).real(),
						eigs.eigenvectors().col(match).row(2).real(),
						eigs.eigenvectors().col(match).row(3).real(),
						eigs.eigenvectors().col(match).row(4).real(),
						eigs.eigenvectors().col(match).row(5).real();

				//std::cout << A << std::endl << std::endl;


				const float sx = stddev.x;
				const float sy = stddev.y;
				const float mx = avg.x;
				const float my = avg.y;

				Eigen::Matrix<double,6,1> vec;

				// WRONG?!
//				vec <<	A(0)*sy*sy,
//						A(1)*sx*sy,
//						A(2)*sx*sx,
//						A(3)*sx*sy*sy - 2*A(0)*mx*sy*sy - A(1)*my*sx*sy,
//						A(4)*sx*sx*sy - 2*A(2)*my*sx*sx - A(1)*mx*sx*sy,
//						A(1)*mx*my*sx*sy + A(0)*mx*my*sy*sy + A(2)*my*my*sx*sx + A(5)*sx*sx*sy*sy - A(3)*mx*sx*sy*sy - A(4)*my*sx*sx*sy; // typo somewhere here

				vec <<
						A(0)*sy*sy,
						A(1)*sx*sy,
						A(2)*sx*sx,
						-2*A(0)*sy*sy*mx - A(1)*sx*sy*my + A(3)*sx*sy*sy,
						-A(1)*sx*sy*mx - 2*A(2)*sx*sx*my + A(4)*sx*sx*sy,
						A(0)*sy*sy*mx*mx + A(1)*sx*sy*mx*my + A(2)*sx*sx*my*my - A(3)*sx*sy*sy*mx - A(4)*sx*sx*sy*my + A(5)*sx*sx*sy*sy	;

				//std::cout << vec << std::endl << std::endl;
				vec = vec / vec.norm();

				//std::cout << vec << std::endl << std::endl;

//				// calculate SVD with FullV (U is NOT needed but compilation fails when using thinU)
//				const Eigen::JacobiSVD<decltype(mat)> svd(mat, Eigen::ComputeFullU | Eigen::ComputeFullV);
//				const Eigen::JacobiSVD<decltype(mat)>::MatrixVType V = svd.matrixV();
//				const Eigen::JacobiSVD<decltype(mat)>::SingularValuesType D = svd.singularValues();

//				// the 6 canonical parameters [A-F] are given by the vector in V corresponding to the smallest singular value
//				// the smallest singular value belongs to the last index in U: thus "5"
//				const Eigen::Matrix<float,6,1> vec = V.col(5);

				return Estimation::fromSVD(vec);


			}

			/** determine ellipse parameters using renormalization [input-weighting together with generalized eigenvalues] */
			template <typename Scalar, typename List> static Estimation getParamsRenormalized(const List& points) {

				using Element = K::Point2<Scalar>;

//				Point2f sum;
//				Point2f sum2;
//				for (const Element p : points) {
//					sum += p;
//					sum2 += p*p;
//				}

//				// calculate average and std-dev for normalization
//				const K::Point2f avg = sum / (float) points.size();
//				const K::Point2f avg2 = sum2 / (float) points.size();
//				//const K::Point2f stddev = K::Point2f(2.5, 4.5);//sqrt(avg2 - avg*avg);		// TODO just for testing
//				const K::Point2f stddev = sqrt(avg2 - avg*avg);

//				// straight line? -> done
//				if (stddev.x == 0 || stddev.y == 0) {return Estimation();}

				// number of points to use
				const size_t num = points.size();



				double W[num];

				// weighting matrix for every input point
				for (size_t i = 0; i < num; ++i) {
					W[i] = 1;//Eigen::Matrix<double, 6,6>::Zero();
				}

				Eigen::Matrix<double, 6, 6> v0e[num];
				Eigen::Matrix<double, 6, 1> result;


				const int maxRuns = 16;

				// perform several iterative steps
				for (int run = 0; run < maxRuns; ++run) {

					Eigen::Matrix<double, 6, 6> M = Eigen::Matrix<double, 6,6>::Zero();
					Eigen::Matrix<double, 6, 6> N = Eigen::Matrix<double, 6,6>::Zero();

					int i = 0;
					for (const Element p : points) {

						// current point
						const float x = (float) p.x;
						const float y = (float) p.y;
						const float f = 1;

						Eigen::Matrix<double, 6, 1> ea;
						ea << x*x, x*y, y*y, f*x, f*y, 1;

						v0e[i] <<
								x*x,	x*y,		0,		f*x,	0,		0,
								x*y,	x*x+y*y,	x*y,	f*y,	f*x,	0,
								0,		x*y,		y*y,	0,		f*y,	0,
								f*x,	f*y,		0,		f*f,	0,		0,
								0,		f*x,		f*y,	0,		f*f,	0,
								0,		0,			0,		0,		0,		0;

						M += W[i] * ea * ea.transpose();
						N += W[i] * v0e[i];
						++i;

					}

					M /= (double)num;
					N /= (double)num;

					// most probably not an ellipse
					if (M(0) != M(0)) {return Estimation();}

					//N = Eigen::Matrix<double, 6, 6>::Identity();

					//std::cout << "M is" << std::endl << M << std::endl << std::endl;
					//std::cout << "N is" << std::endl << N << std::endl << std::endl;



					// http://stackoverflow.com/questions/12672408/generalized-eigenvectors-in-matlab
					Eigen::GeneralizedEigenSolver<Eigen::Matrix<double, 6, 6>> eigs(M, N, true);
					//std::cout << "eigenvalues" << std::endl << eigs.eigenvalues() << std::endl << std::endl;
					//std::cout << "eigenvector" << std::endl << eigs.eigenvectors() << std::endl << std::endl;

					// find the smallest absolute eigenvalue [~ 0.0]
					double matchVal = 999999;
					int match = -1;
					for (int i = 0; i < 6; ++i) {
						const double ev = std::abs( eigs.eigenvalues()(i).real() );
						if (ev < matchVal) {
							match = i;
							matchVal = ev;
						}
					}

					// most probably not an ellipse
					if (i == -1) {return Estimation();}

					// get the corresponding eigenvector
					result <<
							eigs.eigenvectors().col(match).row(0).real(),
							eigs.eigenvectors().col(match).row(1).real(),
							eigs.eigenvectors().col(match).row(2).real(),
							eigs.eigenvectors().col(match).row(3).real(),
							eigs.eigenvectors().col(match).row(4).real(),
							eigs.eigenvectors().col(match).row(5).real();

					// normalize the result vector [length 1.0]
					result = result / result.norm();


					//std::cout << "using eigenvalue " << match << " with value " << matchVal << std::endl << std::endl;
					//std::cout << "result is" << std::endl << result << std::endl << std::endl;

					// update the weight for each input-point to be used in the next run
					for (size_t i = 0; i < num; ++i) {
						W[i] = 1.0 / (result.dot( v0e[i] * result ));
					}

				}

				return Estimation::fromSVD(result);


			}





		};

	}

}

#endif // ELLIPSEESTIMATORMATH_H
