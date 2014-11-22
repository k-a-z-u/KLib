#ifndef K_MATH_FILTER_PARTICLES_PARTICLEASSERTIONS_H
#define K_MATH_FILTER_PARTICLES_PARTICLEASSERTIONS_H

namespace K {

	/** check whether T provides a += operator */
	template <typename T>
	class HasOperatorPlusEq {

		typedef char one;
		typedef long two;

		template <typename C> static one test( decltype(&C::operator+=) ) ;
		template <typename C> static two test(...);

	public:
		enum { value = sizeof(test<T>(0)) == sizeof(one) };

	};

	/** check whether T provides a /= operator */
	template <typename T>
	class HasOperatorDivEq {

		typedef char one;
		typedef long two;

		template <typename C> static one test( decltype(&C::operator/=) ) ;
		template <typename C> static two test(...);

	public:
		enum { value = sizeof(test<T>(0)) == sizeof(one) };

	};


	/** check whether T provides a * operator */
	template <typename T>
	class HasOperatorMul {

		typedef char one;
		typedef long two;

		template <typename C> static one test( decltype(&C::operator*) ) ;
		template <typename C> static two test(...);

	public:
		enum { value = sizeof(test<T>(0)) == sizeof(one) };

	};

	/** check whether T provides an assignment operator */
	template <typename T>
	class HasOperatorAssign{

		typedef char one;
		typedef long two;

		template <typename C> static one test( decltype(&C::operator=) ) ;
		template <typename C> static two test(...);

	public:
		enum { value = sizeof(test<T>(0)) == sizeof(one) };

	};

}

#endif // K_MATH_FILTER_PARTICLES_PARTICLEASSERTIONS_H
