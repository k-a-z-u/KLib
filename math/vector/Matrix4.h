#ifndef K_MATH_VECTOR_MATRIX4_H
#define K_MATH_VECTOR_MATRIX4_H

#include "Matrix.h"

namespace K {

	/**
	 * a 4x4 matrix, usable for 3D graphics that provides
	 * some common functions like rotation and translation.s
	 */
	template <typename T> class Matrix4 : public Matrix<T,4,4> {

	public:

		/** ctor from w*h parameters, separated by ',' */
		template <typename... XX>
		Matrix4(XX... val) : Matrix<T,4,4>(val...) {
			;
		}

		// inherit the assignment operator
		using Matrix<T,4,4>::operator =;

		/** rotate around the X axis */
		Matrix4<T> rotateX( const T rad ) {
			const T c = std::cos(rad);
			const T s = std::sin(rad);
			Matrix4 tmp(
						(T)1,	(T)0,	(T)0,	(T)0,
						(T)0,	c,		-s,		(T)0,
						(T)0,	s,		c,		(T)0,
						(T)0,	(T)0,	(T)0,	(T)1
						);
			return tmp * (*this);
		}

		/** rotate around the Y axis */
		Matrix4<T> rotateY( const T rad ) {
			const T c = std::cos(rad);
			const T s = std::sin(rad);
			Matrix4 tmp(
						c,		(T)0,	s,		(T)0,
						(T)0,	(T)1,	(T)0,	(T)0,
						-s,		(T)0,	c,		(T)0,
						(T)0,	(T)0,	(T)0,	(T)1
						);
			return tmp * (*this);
		}

		/** rotate around the Z axis */
		Matrix4<T> rotateZ( const T rad ) {
			const T c = std::cos(rad);
			const T s = std::sin(rad);
			Matrix4 tmp(
						c,		-s,		(T)0,	(T)0,
						s,		c,		(T)0,	(T)0,
						(T)0,	(T)0,	(T)1,	(T)0,
						(T)0,	(T)0,	(T)0,	(T)1
						);
			return tmp * (*this);
		}

		/** rotate this matrix aroud X,Y, and Z axis */
		Matrix4 rotateXYZ( const T radX, const T radY, const T radZ ) {

			const T cx = std::cos(radX);
			const T sx = std::sin(radX);

			const T cy = std::cos(radY);
			const T sy = std::sin(radY);

			const T cz = std::cos(radZ);
			const T sz = std::sin(radZ);

			Matrix4 tmp(
						cz*cy,	-sz*cx + cz*sy*sx,	 sz*sx + cz*sy*cx,	(T)0,
						sz*cy,	 cz*cx + sz*sy*sx,	-cz*sx + sz*sy*cx,	(T)0,
						-sy,	 cy*sx,				 cy*cx,				(T)0,
						(T)0,	(T)0,				(T)0,				(T)1
						);
			return tmp * (*this);

		}

		/** apply a 3D scaling */
		Matrix4<T> scale( const T x, const T y, const T z ) {
			Matrix4 tmp(
						x,		(T)0,	(T)0,	(T)0,
						(T)0,	y,		(T)0,	(T)0,
						(T)0,	(T)0,	z,		(T)0,
						(T)0,	(T)0,	(T)0,	(T)1
						);
			return tmp * (*this);
		}

		Matrix4<T> move( const T x, const T y, const T z ) {
//			Matrix4 tmp(
//						(T)1, (T)0, (T)0, (T)x,
//						(T)0, (T)1, (T)0, (T)y,
//						(T)0, (T)0, (T)1, (T)z,
//						(T)0, (T)0, (T)0, (T)1
//						);
//			return tmp * (*this);
			this->set(3,0, x);
			this->set(3,1, y);
			this->set(3,2, z);
			return *this;
		}

	};

}

#endif // K_MATH_VECTOR_MATRIX4_H
