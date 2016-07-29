#ifndef K_ASSERTIONS_H
#define K_ASSERTIONS_H

#include "Exception.h"

/**
 * this file provides assertion methods that may be enabled to trace
 * coding errors efficiently during debug or disabled to speed up the
 * code execution for the release build
 */



#ifdef WITH_ASSERTIONS

#define _getMethod()						std::string(__PRETTY_FUNCTION__)
#define _throwAssertion(errorString)		throw K::Exception( "in:\t" + _getMethod() + "\nerror:\t" + errorString )

#define _assertEqual(val1, val2, errorString)			if (val1 != val2) {_throwAssertion(errorString);}
#define _assertTrue(val, errorString)					if (!(val)) {_throwAssertion(errorString);}
#define _assertFalse(val, errorString)					if (val) {_throwAssertion(errorString);}
#define _assertNotNull(val, errorString)				if (!(val)) {_throwAssertion(errorString);}
#define _assertNotNAN(val, errorString)					if (val != val) {_throwAssertion(errorString);}
#define _assertNotEmpty(str, errorString)				if (str.empty()) {_throwAssertion(errorString);}
#define _assertNot0(val, errorString)					if (val == 0) {_throwAssertion(errorString);}
#define _assertBetween(val, min, max, errorString)		if (val < min || val > max) {_throwAssertion(errorString+ " (" + std::to_string(min) + " <= " + std::to_string(val) + " <= " + std::to_string(max) + ")");}
#define _assertNear(val, exp, delta, errorString)		if (std::abs(val-exp) > delta) {_throwAssertion(errorString);}
#else

#define _assertEqual(val1, val2, errorString)
#define _assertTrue(val, errorString)
#define _assertFalse(val, errorString)
#define _assertNotNull(val, errorString)
#define _assertNotNAN(val, errorString)
#define _assertNotEmpty(str, errorString)
#define _assertNot0(val, errorString)
#define _assertBetween(val, min, max, errorString)

#endif

#endif // K_ASSERTIONS_H
