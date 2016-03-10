#ifndef CAMERAINTRINSICS_H
#define CAMERAINTRINSICS_H

#include <eigen3/Eigen/Dense>
#include "../Point2.h"

#include "../../math/optimization/NumOptAlgoGenetic.h"

//	https://en.wikipedia.org/wiki/Camera_resectioning
//	http://biomech.byu.edu/hunter/EXSC662/DLT.aspx
//	http://bardsley.org.uk/wp-content/uploads/2007/02/3d-reconstruction-using-the-direct-linear-transform.pdf
//	https://www.youtube.com/watch?v=ywternCEqSU
//	!!!https://www.youtube.com/watch?v=Ou9Uj75DJX0

namespace K {

	namespace Camera {

		struct Intrinsics {

			float focalX;
			float focalY;
			float centerX;
			float centerY;
			float skew;

			/** ctor */
			Intrinsics(const float fx, const float fy, const float cx, const float cy, const float skew) :
				focalX(fx), focalY(fy), centerX(cx), centerY(cy), skew(skew) {;}

			/** translate the given depthImage */
			void translateDepthImage(const float ix, const float iy, const float iz, float& x, float& y, float& z) {
				z = iz;
				x = (ix - centerX) * z / focalX;
				y = (iy - centerY) * z / focalY;
			}

		} __attribute__((packed));




	};

}

class CameraIntrinsics {

private:

//	/** intrinsic camera parameters */
//	Eigen::Matrix3f params;

public:

//	/** ctor */
//	CameraIntrinsics(const float focalX, const float focalY, const float centerX, const float centerY, const float skewAngle) {
//		params <<
//				focalX,		skewAngle,	centerX,
//				0,			focalY,		centerY,
//				0,			0,			1;
//	}

//	/** apply */
//	Point2<float> (const float x, const float y, const float z) {
//		Eigen::Vector2f = params * Eigen::Vector3f(x,y,z);
//		return Point2<float>(params(0), params(1));
//	}

	struct Intrinsics {
		float focalX;
		float focalY;
		float centerX;
		float centerY;
		float skew;
	} __attribute__((packed));

	struct Translation {
		float x;
		float y;
		float z;
	} __attribute__((packed));

	struct Rotation {
		float x;
		float y;
		float z;
	} __attribute__((packed));

	struct Extrinsics {
		Translation translation;
		Rotation rotation;
	} __attribute__((packed));

	typedef Eigen::Matrix<float, 3, 3> Mat3;
	typedef Eigen::Matrix<float, 3, 1> Vec3;
	typedef Eigen::Matrix<float, 2, 1> Vec2;

	/** get a rotation matrix for the given x,y,z rotation (in radians) */
	static Mat3 getRotation(const Rotation& rot) {
		const float g = rot.x; const float b = rot.y; const float a = rot.z;
		const float a11 = std::cos(a)*std::cos(b);
		const float a12 = std::cos(a)*std::sin(b)*std::sin(g)-std::sin(a)*std::cos(g);
		const float a13 = std::cos(a)*std::sin(b)*std::cos(g)+std::sin(a)*std::sin(g);
		const float a21 = std::sin(a)*std::cos(b);
		const float a22 = std::sin(a)*std::sin(b)*std::sin(g)+std::cos(a)*std::cos(g);
		const float a23 = std::sin(a)*std::sin(b)*std::cos(g)-std::cos(a)*std::sin(g);
		const float a31 = -std::sin(b);
		const float a32 = std::cos(b)*std::sin(g);
		const float a33 = std::cos(b)*std::cos(g);
		Mat3 m;
		m <<
			a11,	a12,	a13,
			a21,	a22,	a23,
			a31,	a32,	a33
		;
		return m;
	}

	static Vec3 getTranslation(const Translation& trans) {
		Vec3 vec;
		vec << trans.x, trans.y, trans.z;
		return vec;
	}

	static Mat3 getIntrinsics(const Intrinsics& intr) {
		Mat3 m;
		m <<	intr.focalX,	intr.skew,		intr.centerX,
				0,				intr.focalY,	intr.centerY,
				0,				0,				1;
		return m;
	}

	/** world <-> image correspondance */
	struct Pair {
		Vec3 world;
		Vec2 img;
		Pair(const float wx, const float wy, const float wz, const float ix, const float iy) {
			world << wx, wy, wz;
			img << ix, iy;
		}
	};

	struct Plane {
		std::vector<Pair> pairs;
	};

	template <int numPlanes> static void calibrate(std::vector<Plane> planes) {

		struct Params {
			Intrinsics intr;
			Extrinsics extr[numPlanes];
		} __attribute__((packed));

		Params params;

		memset(&params, 0, sizeof(params));


		struct Func {

			std::vector<Plane> planes;

			Func(std::vector<Plane>& planes) : planes(planes) {;}

			float getError(const float* ptr) const {

				Params* params = (Params*) ptr;
				//params->intr.skew = 0;
				//params->intr.focalX = params->intr.focalY;
				const Mat3 intr = getIntrinsics(params->intr);

				float err = 0;
				int cnt = 0;

				for (int i = 0; i < numPlanes; ++i) {

					const Plane& plane = planes[i];
					const Mat3 rot = getRotation(params->extr[i].rotation);
					const Vec3 trans = getTranslation(params->extr[i].translation);

					for (const Pair& p : plane.pairs) {
						Vec3 w = p.world*30*12;		// TODO: remove
						Vec3 v = intr * (rot * (w + trans));
						//Vec2 img; img << (v(0) / v(2)), (v(1) / v(2));
						//img << v(0), v(1);
						//err += (p.img - img).norm();
						const float dx = v(0) - p.img(0);
						const float dy = v(1) - p.img(1);
						const float dz = v(2) - 1;
						err += std::sqrt(dx*dx + dy*dy + dz*dz);
						++cnt;
					}


					// prevent negative Z
					if (params->extr[i].translation.z < 0) {err += 9999999;}

				}

				err/= cnt;

				static float cerr = 999999999;
				if (err < cerr) {
					std::cout << "err: " << err << std::endl;
					cerr = err;
				}
				return err;

			}

		} func(planes);

		K::NumOptAlgoGenetic<float> gen(5 + 6*numPlanes);

		gen.setPopulationSize(1000);
		gen.setMaxIterations(100);
		gen.setValRange(0.02f);
		gen.setMutation(0.4f);
		gen.setElitism(0.1f);

		gen.calculateOptimum(func, (float*) &params);


		for (int i = 0; i < numPlanes; ++i) {
			std::cout << "plane " << i << std::endl;
			std::cout << "\trot: " << params.extr[i].rotation.x << ", " << params.extr[i].rotation.y << ", " << params.extr[i].rotation.z << std::endl;
			std::cout << "\ttra: " << params.extr[i].translation.x << ", " << params.extr[i].translation.y << ", " << params.extr[i].translation.z << std::endl;
		}
		std::cout << "intr: " << params.intr.centerX << ", " << params.intr.centerY << " : " << params.intr.focalX << ", " << params.intr.focalY << " : " << params.intr.skew << std::endl;
		std::cout << "err: " << func.getError((float*) &params) << std::endl;



	}



};

#endif // CAMERAINTRINSICS_H
