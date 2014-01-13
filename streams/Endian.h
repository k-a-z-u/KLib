/*
 * Endian.h
 *
 *  Created on: Dec 19, 2013
 *      Author: kazu
 */

#ifndef ENDIAN_H_
#define ENDIAN_H_

namespace K {

#define K_SWAP_16(x)	__builtin_bswap16(x)
#define K_SWAP_32(x)	__builtin_bswap32(x)
#define K_SWAP_64(x)	__builtin_bswap64(x)

// detect endianness
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define K_IS_LITTLE_ENDIAN
#else

#endif

/**
 * provides endianness conversion
 */
class Endian {

public:

	/** convert the given word from host's endianness to little-endian */
	static uint16_t toLittleEndian(uint16_t in) {
#ifdef K_IS_LITTLE_ENDIAN
		return in;
#else
		return K_SWAP_16(in);
#endif
	}

	/** convert the given word from little-endian to host's endianness */
	static uint16_t fromLittleEndian(uint16_t in) {
		return toLittleEndian(in);
	}

	/** convert the given int from host's endianness to little-endian */
	static uint32_t toLittleEndian(uint32_t in) {
#ifdef K_IS_LITTLE_ENDIAN
		return in;
#else
		return K_SWAP_32(in);
#endif
	}

	/** convert the given int from little-endian to host's endianness */
	static uint32_t fromLittleEndian(uint32_t in) {
		return toLittleEndian(in);
	}


	/** convert the given int from host's endianness to little-endian */
	static uint64_t toLittleEndian(uint64_t in) {
#ifdef K_IS_LITTLE_ENDIAN
		return in;
#else
		return K_SWAP_64(in);
#endif
	}

	/** convert the given int from little-endian to host's endianness */
	static uint64_t fromLittleEndian(uint64_t in) {
		return toLittleEndian(in);
	}




	/** convert the given float from host's endianness to little-endian */
	static float toLittleEndian(float in) {
#ifdef K_IS_LITTLE_ENDIAN
		return in;
#else
		union {
			float f;
			uint32_t i;
		} f;
		f.f = in;
		f.i = K_SWAP_32(f.i);
		return f.f;
#endif
	}

	/** convert the given float from little-endian to host's endianness */
	static float fromLittleEndian(float in) {
		return toLittleEndian(in);
	}


	/** convert the given double from host's endianness to little-endian */
	static double toLittleEndian(double in) {
#ifdef K_IS_LITTLE_ENDIAN
		return in;
#else
		union {
			double d;
			uint64_t i;
		} d;
		d.d = in;
		d.i = K_SWAP_64(d.i);
		return d.d;
#endif
	}

	/** convert the given float from little-endian to host's endianness */
	static double fromLittleEndian(double in) {
		return toLittleEndian(in);
	}

	/** convert the given big-endian to host's endianness */
	static uint32_t bigToHost(uint32_t in) {
#ifdef K_IS_LITTLE_ENDIAN
		return K_SWAP_32(in);
#else
		return val
#endif
	}
	
	/** convert the given big-endian to host's endianness */
	static uint16_t bigToHost(uint16_t in) {
#ifdef K_IS_LITTLE_ENDIAN
		return K_SWAP_16(in);
#else
		return val
#endif
	}
	
	
};


}


#endif /* ENDIAN_H_ */
