#ifndef K_CV_CIRCLEDETECTION3_H
#define K_CV_CIRCLEDETECTION3_H

#include "PlaneDetection3.h"
#include "CircleDetection2.h"

#include "../../geo/Circle3.h"
#include "../../geo/Plane3.h"
#include "../../geo/Matrix.h"


namespace K {

	/**
	 * estime a 3D circle given some points that are part of the circle
	 */
	template <typename Scalar> class CircleEstimator3 {

	public:

		/**
		 * ransac-based parameter estimation.
		 * ransac is only used for the plane the points of the circle have to belong to.
		 * all points belonging to the plane are then used for the circle estimation.
		 * so, no extra ransac for the circle is needed.
		 */
		class RANSAC {

			private:

				typename PlaneEstimator3<Scalar>::RANSAC planeRansac;

			public:

				/** set the number of runs to perform */
				void setNumRuns(const int numRuns) {planeRansac.setNumRuns(numRuns);}

				/** set the minimum number of inliers [0.0:1.0] needed for a plane to be accepted */
				void setMinMatchRate(const float rate) {planeRansac.setMinMatchRate(rate);}

				/** set the maximum distance for a point from the plane to count as inlier */
				void setMaxDistance(const float dist) {planeRansac.setMaxDistance(dist);}

				/** get the maximum distance for a point from the plane to count as inlier */
				float getMaxDistance() const {return planeRansac.getMaxDistance();}

				/** get estimation */
				Circle3<Scalar> get(const std::vector<Point3<Scalar>>& points) {

					// number of points to use
					const int num = (int) points.size();

					// sanity check
					_assertTrue(num >= 10, "needs at least 10 points of the circle to estimate with RANSAC");

					// estimate the plane the to-be-detected circle probably belongs to
					const typename Plane3<Scalar>::CoordinateParams plane = planeRansac.get(points);

					// construct vector of all inliers (that belong to the plane)
					std::vector<Point3<Scalar>> inliers;
					for (const Point3<Scalar>& p : points) {
						if (plane.getDistance(p) <= planeRansac.getMaxDistance()) {
							inliers.push_back(p);
						}
					}

					// use this inliers + their plane for circle estimation
					return getParams(plane, inliers);

				}


		};


	public:

		template <typename List> static Circle3<float> getParams(const List& points) {

			// number of points to use
			const int num = (int) points.size();

			// sanity check
			_assertTrue(num >= 3, "needs at least 3 points of the circle to estimate");

			// estimate the plane the given points denote (this is the plane the circle belongs to)
			Plane3<float>::CoordinateParams plane = PlaneEstimator3<Scalar>::getParams(points);
			//Plane3<float>::NormalParams plane = PlaneEstimator3<Scalar>::getNormalParams(points);

			// estimate
			return getParams(plane, points);

		}


	private:

		/**
		 * estimate the circle-parameters for the given points,
		 * that are known to belong to the provided plane.
		 *
		 * this will rotate all points to be axis aligned (normal: (0,0,1))
		 * hereafter its possible to use the 2D circle-detection as the z-component is eliminated
		 */
		template <typename List> static Circle3<float> getParams(const Plane3<float>::CoordinateParams& plane, const List& points) {

			// get a matrix that rotates the plane into another plane that is axis align and has the normal (0,0,1)
			Eigen::Vector3f planeNormal; planeNormal << plane.normal.x, plane.normal.y, plane.normal.z;
			Eigen::Vector3f targetNormal; targetNormal << 0,0,1;
			Eigen::Matrix3f rotMat = MatrixHelper::getRotationMatrix( planeNormal, targetNormal );

			// all points, aligned to the (x,y) plane
			std::vector<Point2<Scalar>> aligned;

			// align each input point
			Point3<Scalar> sum;
			for (const Point3<Scalar>& p : points) {

				// rotate it into the (0,0,1) plane ->
				const Point3<Scalar> pRot = rotMat * (p);

				// update the sum (to calculate the average)
				sum += pRot;

				// append
				aligned.push_back(Point2<Scalar>(pRot.x, pRot.y));

			}

			// average z-value of all aligned input values
			const Point3<Scalar> avg = sum / (float) aligned.size();

			// now we can perform a 2D parameter estimation using (x,y) coordinate [z values are all the same]
			const Circle2<float>::CanonicalParams circle2 = CircleEstimator2<Scalar>::getParams(aligned);

			// sanity check
			if (circle2.getRadius() != circle2.getRadius()) {
				std::cout << "NaN radius!" << std::endl;
				return Circle3<float>();
			}

			// convert to geometric parameters
			const Circle2<float> circle2Geo = circle2.toGeometric();

			// now, we need to "un-rotate" the 2D estimation to get the 3D estimation
			// as z-component we use the average z-value the points had after the axis-alignment
			const Eigen::Matrix3f iMat = rotMat.inverse();
			const Point3<Scalar> center3 = (iMat * Point3<Scalar>(circle2Geo.center.x, circle2Geo.center.y, avg.z));
			const Point3<Scalar> normal3 = iMat * Point3<Scalar>(0,0,1);
			const float radius3 = circle2Geo.radius;

			// done
			return Circle3<float>(center3, normal3, radius3);

		}

	};

}

#endif // K_CV_CIRCLEDETECTION3_H
