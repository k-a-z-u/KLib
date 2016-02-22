/*
 * NumOptVector.h
 *
 *  Created on: Sep 3, 2013
 *      Author: Frank Ebner
 */

#ifndef K_MATH_OPT_NUMOPTVECTOR_H_
#define K_MATH_OPT_NUMOPTVECTOR_H_

#include <cassert>
#include <cstring>
#include <iostream>

#include <array>


#ifdef _WIN32
#pragma warning( disable : 4351 )
#endif

namespace K {

	/** convenience class to initialize the NumOptVector */
	template <int size> struct NumOptVectorData {
		double values[size];
	};


	/**
	 * represents a (fixed-size) vector
	 * used within numerical-optimization backend for e.g.
	 * 	- arguments
	 * 	- gradients
	 *
	 */
	template <int size> class NumOptVector {

	public:

		/** ctor */
		NumOptVector() : data() {;}

		/** ctor with data */
                NumOptVector(NumOptVectorData<size> init) {
                        //assert(vals.size() == size);
                        std::copy(init.values, init.values+size, data);
                }


		/** get the length (euclid) of this vector */
		double getLength() const {
			double sum = 0;
			for (unsigned int i = 0; i < size; ++i) {
				sum += data[i] * data[i];
			}
			return sqrt(sum);
		}


		/** normalize this vector */
		void normalize() {
			double len = getLength();
			*this /= len;
		}

		/** read/write array access */
		double& operator [] (const int idx) {return data[idx];}

		/** read array access */
		const double& operator [] (const int idx) const {return data[idx];}

		/** data access */
		const double* getData() const {return data;}


		/** -------------------------------- DOUBLE -------------------------------- */

		/** add double to vector */
		NumOptVector<size> operator + (double rhs) const {
			NumOptVector<size> copy(*this);
			copy += rhs;
			return copy;
		}

		/** sub double to vector */
		NumOptVector<size> operator - (double rhs) const {
			NumOptVector<size> copy(*this);
			copy -= rhs;
			return copy;
		}

		/** multiply double with vector */
		NumOptVector<size> operator * (double rhs) const {
			NumOptVector<size> copy(*this);
			copy *= rhs;
			return copy;
		}

		/** divide vector by double */
		NumOptVector<size> operator / (double rhs) const {
			NumOptVector<size> copy(*this);
			copy /= rhs;
			return copy;
		}

		/** add double to vector */
		NumOptVector<size>& operator += (double rhs) {
			for (unsigned int i = 0; i < size; ++i) {data[i] += rhs;}
			return *this;
		}

		/** sub double from vector */
		NumOptVector<size>& operator -= (double rhs) {
			for (unsigned int i = 0; i < size; ++i) {data[i] -= rhs;}
			return *this;
		}

		/** multiply double with vector */
		NumOptVector<size>& operator *= (double rhs) {
			for (unsigned int i = 0; i < size; ++i) {data[i] *= rhs;}
			return *this;
		}

		/** divide vector by double */
		NumOptVector<size>& operator /= (double rhs) {
			for (unsigned int i = 0; i < size; ++i) {data[i] /= rhs;}
			return *this;
		}



		/** -------------------------------- VECTOR -------------------------------- */

		/** add double to vector */
		NumOptVector<size> operator + (const NumOptVector<size>& rhs) const {
			NumOptVector<size> copy(*this);
			copy += rhs;
			return copy;
		}

		/** sub double to vector */
		NumOptVector<size> operator - (const NumOptVector<size>& rhs) const {
			NumOptVector<size> copy(*this);
			copy -= rhs;
			return copy;
		}

		/** add vector to vector */
		NumOptVector<size>& operator += (const NumOptVector<size>& rhs) {
			for (unsigned int i = 0; i < size; ++i) {data[i] += rhs.data[i];}
			return *this;
		}

		/** sub vector from vector */
		NumOptVector<size>& operator -= (const NumOptVector<size>& rhs) {
			for (unsigned int i = 0; i < size; ++i) {data[i] -= rhs.data[i];}
			return *this;
		}


		/** -------------------------------- COPY -------------------------------- */

		/** assignment */
		NumOptVector<size>& operator = (const NumOptVector<size>& other) {
			memcpy(this->data, other.data, sizeof(data));
			return *this;
		}

		/** copy ctor */
		NumOptVector(const NumOptVector& other) {
			memcpy(this->data, other.data, sizeof(data));
		}


		/** output */
		friend std::ostream& operator << (std::ostream& out, const NumOptVector<size>& ref) {
			out << '[';
			for (unsigned int i = 0; i < size; ++i) {
				out << ref[i];
				if (i < size-1) {out << ", ";}
			}
			out << ']';
			return out;
		}


	private:

		/** the data array */
		double data[size];

	};

}

#endif /* K_MATH_OPT_NUMOPTVECTOR_H_ */
