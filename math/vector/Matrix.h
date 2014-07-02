#ifndef K_MATH_VECTOR_MATRIX_H
#define K_MATH_VECTOR_MATRIX_H

#include <iosfwd>
#include "Vector.h"

namespace K {

	template<typename T, int w, int h> class Matrix {

	public:

		/** empty ctor */
		Matrix() : data() {
			;
		}

		/** ctor from data array */
		Matrix(T* val) {
			memcpy(data, val, sizeof(data));
		}

		/** ctor from w*h parameters, separated by ',' */
		template <typename... XX>
		Matrix(XX... val) : data{val...} {
			;
		}

		/** copy ctor */
		Matrix(const Matrix<T,w,h>& other) {
			memcpy(data, other.data, sizeof(data));
		}


		/** set the value for the given field */
		void set(unsigned int x, unsigned int y, T value) {
			assert(x < w);
			assert(y < h);
			data[x+y*w] = value;
		}

		/** get the value behind the given field */
		T get(const unsigned int x, const unsigned int y) const {
			assert(x < w);
			assert(y < h);
			return data[x+y*w];
		}

		/** set all values to zero */
		void clear() {
			memset(data, 0, sizeof(data));
		}

		/** fill this matrix with identity data */
		void identity() {
			clear();
			unsigned int max = (w<h) ? (w) : (h);
			for (unsigned int i = 0; i < max; ++i) {
				set(i, i, 1);
			}
		}


		/** multiply the matrix by a constant value */
		const Matrix<T,w,h>& operator *= ( const T real ) {
			for (unsigned int i = 0; i < w*h; ++i) { data[i] *= real; }
			return *this;
		}

		/** divide the matrix by a constant value */
		const Matrix<T,w,h>& operator /= ( const T real ) {
			for (unsigned int i = 0; i < w*h; ++i) { data[i] /= real; }
			return *this;
		}

		/** add a constant value to the matrix */
		const Matrix<T,w,h>& operator += ( const T real ) {
			for (unsigned int i = 0; i < w*h; ++i) { data[i] += real; }
			return *this;
		}

		/** remove a constant value from the matrix */
		const Matrix<T,w,h>& operator -= ( const T real ) {
			for (unsigned int i = 0; i < w*h; ++i) { data[i] -= real; }
			return *this;
		}


		/** write to output stream */
		friend std::ostream& operator << (std::ostream& out, const Matrix& m) {
			out << "matrix(\n";
			for (unsigned int y = 0; y < h; ++y) {
				for (unsigned int x = 0; x < w; ++x) {
					out << m.get(x,y) << ' ';
				}
				out << "\n";
			}
			out << ')';
			return out;
		}

		/** multiply matrix by the given vector */
		friend Vector<T, h> operator * (Matrix<T, w, h>& mat, Vector<T, w>& vec) {
			Vector<T, h> res;
			for (unsigned int x = 0; x < w; ++x) {
				for (unsigned int y = 0; y < h; ++y) {
					res[x] += vec[y] * mat.get(x,y);
				}
			}
			return res;
		}

		/** multiply this matrix by the given matrix */
		const Matrix<T,w,h>& operator *= ( const Matrix<T,w,h>& other ) {

			// copy my current values
			T myCopy[w*h];
			memcpy(myCopy, data, sizeof(data));
			this->clear();

			// multiply
			for ( unsigned int x = 0; x < w; ++x ) {
				for ( unsigned int y = 0; y < h; ++y ) {
					for ( unsigned int k = 0; k < w; ++k ) {
						data[x+y*w] += (myCopy[x+k*w] * other.data[k+y*w]);
					}
				}
			}

			return *this;

		}

		/** multiply this matrix by the given matrix */
		Matrix<T,w,h> operator * ( const Matrix<T, w, h>& other ) {
			Matrix cpy = *this;
			cpy *= other;
			return cpy;
		}


		/** compare other matrix to this matrix */
		bool operator == ( const Matrix<T,w,h>& other ) const {
			for (unsigned int i = 0; i < w*h; ++i) {
				if (data[i] != other.data[i]) {return false;}
			}
			return true;
		}


	protected:

		/** the matrix's data */
		T data[w*h];

	};

}

#endif // K_MATH_VECTOR_MATRIX_H
