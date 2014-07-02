#ifndef K_MATH_VECTOR_VECTOR4_H_
#define K_MATH_VECTOR_VECTOR4_H_

#include "Vector.h"

namespace K {

	/**
	 * special vector version with 4 fields (x,y,z,w) e.g. for 3D stuff
	 */
	template <typename type> class Vector4 : public Vector<type, 4> {

	public:

//		/** ctor */
//		Vector4() : Vector<type,4>() {
//			;
//		}

//		/** ctor */
//		Vector4(type x, type y, type z, type w) : Vector<type,4>(x,y,z,w) {
//			;
//		}

		// inherit the assignment operator
		using Vector<type,4>::operator =;
		using Vector<type,4>::values;

		type x() const {return values[0];}
		type y() const {return values[1];}
		type z() const {return values[2];}
		type w() const {return values[3];}

		void x(type x) {values[0] = x;}
		void y(type y) {values[1] = y;}
		void z(type z) {values[2] = z;}
		void w(type w) {values[3] = w;}

	};
}

#endif // K_MATH_VECTOR_VECTOR4_H_
