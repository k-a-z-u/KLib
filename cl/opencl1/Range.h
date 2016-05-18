#ifndef K_CL_RANGE_H
#define K_CL_RANGE_H

#include "Base.h"

namespace K {
	namespace CL {

		struct Range {

			size_t x;
			size_t y;
			size_t z;

			Range(const size_t x) : x(x), y(0), z(0) {;}
			Range(const size_t x, const size_t y) : x(x), y(y), z(0) {;}
			Range(const size_t x, const size_t y, const size_t z) : x(x), y(y), z(z) {;}

		};

	}
}

#endif // K_CL_RANGE_H
