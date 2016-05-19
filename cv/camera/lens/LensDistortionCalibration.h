#ifndef LENSDISTORTIONCALIBRATION_H
#define LENSDISTORTIONCALIBRATION_H

#include "../../../geo/Point2.h"
#include <vector>

#include "LensDistortionRadial.h"
#include "../../../math/optimization/NumOptAlgoDownhillSimplex.h"
#include "../../../math/optimization/NumOptAlgoGenetic.h"


namespace K {

	class LensDistortionCalibration {

	public:

		struct Path : std::vector<Point2f> {

			Path& add(const Point2f& p) {push_back(p); return*this;}

			void center(const Point2f imageSize) {
				for (Point2f& p : *this) { p /= imageSize; p -= Point2f(0.5, 0.5); }
			}

		};




		/** get the error induced by the given path */
		template <typename T> static float getError(const Path& path, const T* params, const int num) {

			float cumErr = 0;

			// start and end position of the path
			const Point2f start = path.front();
			const Point2f end = path.back();
			const Point2f dir = end - start;

			// sanity check
			_assertBetween(start.x, -0.5, +0.5, "start.x out of range");
			_assertBetween(start.y, -0.5, +0.5, "start.y out of range");
			_assertBetween(end.x, -0.5, +0.5, "end.x out of range");
			_assertBetween(end.y, -0.5, +0.5, "end.y out of range");

			// calculate the angular error
			for (size_t i = 1; i < path.size()-1; ++i) {
				const Point2f p0 = LensDistortionRadial::distort(path[i-1], params, num);
				const Point2f p1 = LensDistortionRadial::distort(path[i], params, num);
				const Point2f p2 = LensDistortionRadial::distort(path[i+1], params, num);
				const float isAngle01 = (p1-p0).getDirection();
				const float isAngle12 = (p2-p1).getDirection();
				const float diff = (isAngle01 - isAngle12) * 10;
				const float err = diff*diff;
				cumErr += err;
			}

			// calculate the distance std-deviation (same distance for all)
			double sum = 0;
			double sumSq = 0;
			for (size_t i = 0; i < path.size()-1; ++i) {
				const Point2f p1 = LensDistortionRadial::distort(path[i], params, num);
				const Point2f p2 = LensDistortionRadial::distort(path[i+1], params, num);
				const float dist = p1.getDistance(p2) * 10;
				sum += dist;
				sumSq += dist*dist;
			}

			sum /= (path.size() - 1);
			sumSq /= (path.size() - 1);
			const double distSigma = std::sqrt(sumSq - (sum*sum));
			if (distSigma != distSigma) {
				cumErr += distSigma;
			}
			cumErr += distSigma;

			return cumErr;

		}

	public:

//		/**
//		 * calculates "num" parameters to undistort the given distorted coordinates
//		 * into their undistorted counterparts
//		 *
//		 * @param distortedCoordinates 2D coordinates within the distorted image
//		 * @param undistoredCoordinates 2D should-be coordinates (= undistorted)
//		 * @param num the number of parameters to estimate
//		 * @return
//		 */
//		std::vector<float> calibrate(const std::vector<Point2f>& distortedCoordinates, const std::vector<Point2>& undistoredCoordinates, const int num) {



//		}

		static std::vector<float> calibrate(std::vector<Path>& paths, const Point2f& imageSize, const int num) {

//			class Func : public NumOptFunction<2> {
//				const std::vector<Path>& paths;
//			public:
//				Func(const std::vector<Path>& paths) : paths(paths){;}
//				virtual double getValue(const NumOptVector<2>& args) const override {

//					double cumErr = 0;

//					for (const Path& path : paths) {
//						const float err = getError(path, &args[0], 2);
//						cumErr += err;
//					}

//					std::cout << "err: " << cumErr << std::endl;
//					std::cout << "\t" << args[0] << ":" << args[1] << std::endl;
//					return cumErr;

//				}
//			} func(paths);

//			NumOptVector<2> vec;
//			NumOptAlgoDownhillSimplex<2> opt;

//			for (Path& p : paths) {
//				p.center(imageSize);
//			}

//			opt.calculateOptimum(func, vec);
//			return std::vector<float>(&vec[0], &vec[num]);

			return std::vector<float>();

		}

		void test() {

	//		std::vector<Point2f> dist;
	//		std::vector<Point2f> undist;

	//		dist.push_back(Point2f(152,95));		undist.push_back(Point2f(0,0));
	//		dist.push_back(Point2f(172,96));		undist.push_back(Point2f(1,0));
	//		dist.push_back(Point2f(192,97));		undist.push_back(Point2f(3,0));
	//		dist.push_back(Point2f(213,99));		undist.push_back(Point2f(4,0));
	//		dist.push_back(Point2f(234,101));		undist.push_back(Point2f(5,0));
	//		dist.push_back(Point2f(254,104));		undist.push_back(Point2f(6,0));
	//		dist.push_back(Point2f(275,106));		undist.push_back(Point2f(7,0));
	//		dist.push_back(Point2f(295,108));		undist.push_back(Point2f(8,0));








		}



	};

}

#endif // LENSDISTORTIONCALIBRATION_H
