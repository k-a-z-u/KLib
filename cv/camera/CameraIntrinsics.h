#ifndef CAMERAINTRINSICS_H
#define CAMERAINTRINSICS_H

#include <eigen3/Eigen/Dense>
#include "../Point2.h"

//	https://en.wikipedia.org/wiki/Camera_resectioning
//	http://biomech.byu.edu/hunter/EXSC662/DLT.aspx

class CameraIntrinsics {

private:

	/** intrinsic camera parameters */
	Eigen::Matrix3f params;

public:

	/** ctor */
	CameraIntrinsics(const float focalX, const float focalY, const float centerX, const float centerY, const float skewAngle) {
		params <<
				focalX,		skewAngle,	centerX,
				0,			focalY,		centerY,
				0,			0,			1;
	}

	/** apply */
	Point2<float> (const float x, const float y, const float z) {
		Eigen::Vector2f = params * Eigen::Vector3f(x,y,z);
		return Point2<float>(params(0), params(1));
	}



};

#endif // CAMERAINTRINSICS_H
