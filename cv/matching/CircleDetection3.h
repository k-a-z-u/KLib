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

		template <typename List> static Circle3<float> getParams(const List& points) {

			// number of points to use
			const int num = (int) points.size();

			// sanity check
			_assertTrue(num >= 3, "needs at least 3 points of the circle to estimate");

			// estimate the plane the given points denote (this is the plane the circle belongs to)
			Plane3<float>::CoordinateParams planeX = PlaneEstimator3<Scalar>::getParams(points);
			Plane3<float>::NormalParams plane = PlaneEstimator3<Scalar>::getNormalParams(points);

			std::cout << "*" << planeX.normal.asString() << std::endl;
			std::cout << "*" << plane.normal.asString() << std::endl;


			// get a matrix that rotates the circle from its plane into the (x,y) plane
			Eigen::Vector3f planeNormal; planeNormal << plane.normal.x, plane.normal.y, plane.normal.z;
			Eigen::Vector3f targetNormal; targetNormal << 0,0,1;
			Eigen::Matrix3f rotMat = MatrixHelper::getRotationMatrix( planeNormal, targetNormal );

			// all points, aligned to the (x,y) plane
			std::vector<Point2<Scalar>> aligned;

			// align each input point
			Point3<Scalar> sum;
			for (const Point3<Scalar>& p : points) {

				// rotate it into the (0,0,1) plane ->
				const Point3<Scalar> pRot = rotMat * (p);//-plane.center);

				// z-value should now be (approx) zero for all points
				//_assertNear(0, pRot.z, 0.1, "z value is too far from zero");
				sum += pRot;

				// append
				aligned.push_back(Point2<Scalar>(pRot.x, pRot.y));

			}

			// average z-value of all aligned input values
			const Point3<Scalar> avg = sum / (float) aligned.size();
			std::cout << "#" << avg.asString() << std::endl;

			// now we can perform a 2D parameter estimation using (x,y) coordinate [z values are all the same]
			const Circle2<float>::CanonicalParams circle2 = CircleEstimator2<Scalar>::getParams(aligned);

			if (circle2.getRadius() != circle2.getRadius()) {
				std::cout << "NaN radius!" << std::endl;
				return Circle3<float>();
			}

			const Circle2<float> circle2Geo = circle2.toGeometric();

			// we need to un-Rotate the 2D estimation to get the correct 3D estimation
			const Eigen::Matrix3f iMat = rotMat.inverse();
			const Point3<Scalar> center3 = (iMat * Point3<Scalar>(circle2Geo.center.x, circle2Geo.center.y, avg.z));// + plane.center;
			const Point3<Scalar> normal3 = iMat * Point3<Scalar>(0,0,1);
			const float radius3 = circle2Geo.radius;

			// ensure the normal points upwards [is exactly the same.. just turned upside down.. thats the degree of freedom we have anyways]
			//const Point3<Scalar> aNormal3 = (normal3.z > 0) ? (normal3) : (-normal3);	 // AND WHAT SHOULD WE DO FOR -x or -y?!....

			// done
			return Circle3<float>(center3, normal3, radius3);

		}

	};

}

#endif // K_CV_CIRCLEDETECTION3_H
