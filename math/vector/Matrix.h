#ifndef K_MATH_VECTOR_MATRIX_H
#define K_MATH_VECTOR_MATRIX_H

#include <iosfwd>
#include "Vector.h"

namespace K {

	template<typename T, int w, int h> class Matrix {

	public:

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

		void identity() {
			clear();
			int max = (w<h) ? (w) : (h);
			for (unsigned int i = 0; i < max; ++i) {
				set(i, i, 1);
			}
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
			for (unsigned int y = 0; y < h; ++y) {
				for (unsigned int x = 0; x < w; ++x) {
					res[y] += vec[x] * mat.get(x,y);
				}
			}
			return res;
		}

	private:

		/** the matrix's data */
		T data[w*h];

	};

}

#endif // K_MATH_VECTOR_MATRIX_H
