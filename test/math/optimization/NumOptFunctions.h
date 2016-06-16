#ifndef NUMOPTFUNCTIONS_H
#define NUMOPTFUNCTIONS_H

namespace Opt {

	template <typename T> static T pow2(const T val) {return val*val;}

	/** f(x) = y. max: y=0 for x=0 */
	static float fSimple(const float* x) {
		return	pow2(x[0]/4);
	}

	/** f(x) = y. huge noise. max: y=-4 for x=0 */
	static float fNoise0(const float* x) {
		return
				-(
					3.0f +
					1.0f * std::cos(x[0]*3) +
					1.0f * std::cos(x[0]*13) -
					pow2(x[0]/4)
				);
	}

	/** f(x) = y. very noisy. max: y=-4 for x=0 */
	static float fNoise1(const float* x) {
		return
				-(
					3.0f +
					0.5f * std::cos(x[0]*3) +
					0.5f * std::cos(x[0]*13) -
					pow2(x[0]/4)
				);
	}

	/** f(x) = y. somewhat noisy. max: y=-3.5 for x=0 */
	static float fNoise2(const float* x) {
		return
				-(
					3.00f +
					0.25f * std::cos(x[0]*3) +
					0.25f * std::cos(x[0]*13) -
					pow2(x[0]/4)
				);
	}


	/** f(x,y) = z. minimum: f(1,1) = 0 */
	static float fRosenbrock (const float* data) {
			return std::pow(1 - data[0], 2) + 100 * std::pow( (data[1] - data[0]*data[0]), 2);
	};

	/**
	 * f ( 3.0 , 2.0 ) = 0.0
	 * f ( − 2.805118 , 3.131312 ) = 0.0
	 * f ( − 3.779310 , − 3.283186 ) = 0.0
	 * f ( 3.584428 , − 1.848126 ) = 0.0
	 */
	static float fHimmelblau (const float* data) {
		return pow( (data[0]*data[0] + data[1] - 11), 2) + pow( (data[0]+data[1]*data[1]-7), 2);
	};


}

#endif // NUMOPTFUNCTIONS_H
