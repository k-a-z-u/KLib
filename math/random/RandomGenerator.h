#ifndef K_MATH_RANDOM_RANDOMGENERATOR_H
#define K_MATH_RANDOM_RANDOMGENERATOR_H

#include <random>
#include <chrono>

#ifdef USE_FIXED_SEED
	#define RANDOM_SEED 1234
#else
	#define RANDOM_SEED ( std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1) )
#endif

class RandomGenerator : public std::minstd_rand {

public:

	/** ctor with default seed */
	RandomGenerator() : std::minstd_rand(RANDOM_SEED) {;}

	/** ctor with custom seed */
	RandomGenerator(result_type) : std::minstd_rand(RANDOM_SEED) {;}

};

#endif // K_MATH_RANDOM_RANDOMGENERATOR_H
