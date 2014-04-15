#ifndef TESTDSPHELPER_H
#define TESTDSPHELPER_H

#include "../../../math/constants.h"

namespace K {

	class TestDSPHelper {

	public:

		/** fill array with simple sine function */
		template <typename type> static void fillSine(type arr[], unsigned int len) {
			for (unsigned int i = 0; i < len; ++i) {
				type t = (type) i / (type) len;
				arr[i] = (type) ( sin(2 * K::PI * t) + sin(2 * K::PI * t * 8) );
			}
		}

	};

}

#endif // TESTDSPHELPER_H
