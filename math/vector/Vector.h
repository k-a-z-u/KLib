#ifndef K_MATH_VECTOR_VECTOR_H_
#define K_MATH_VECTOR_VECTOR_H_

#include <initializer_list>
#include <cassert>
#include <iosfwd>

namespace K {

	/**
	 * represents a simple vector with some basic methods
	 */
	template <typename type, int dimension> class Vector {


	public:

		/** empty ctor */
		Vector() : values() {
			;
		}

		/** ctor from initializer value */
		Vector(const std::initializer_list<type> list) : values() {
			assert(list.size() == dimension);
			for (unsigned int i = 0; i < list.size(); ++i) {
				values[i] = *(list.begin() + i);
			}
		}

		/** wrap the vector around the given external data array */
		Vector(type* data) : values(data) {
			;
		}

		/** ctor from w*h parameters, separated by ',' */
		template <typename... XX>
		Vector(XX... val) : values{val...} {
			;
		}

		/** copy ctor */
		Vector(const Vector& other) : values() {
			memcpy(values, other.values, sizeof(values));
		}

		/** copy ctor */
		Vector(const type other[]) : values() {
			memcpy(values, other, sizeof(values));
		}



		/** assign the given data to this vector (copy) */
		Vector& operator = ( const type other[] ) {
			memcpy(values, other, sizeof(values));
			return *this;
		}

		/** assign the given data to this vector (copy) */
		Vector& operator = ( const Vector& other ) {
			memcpy(values, other.values, sizeof(values));
			return *this;
		}



		/** set the value for the given index (0=x, 1=z, ...). performs out-of-bounds check */
		void set(unsigned int idx, type val) {
			assert(idx < dimension);
			this->values[idx] = val;
		}

		/** get the value for the given index. performs out-of-bounds check */
		type get(unsigned int idx) {
			assert(idx < dimension);
			return this->values[idx];
		}

		/** get the length of this vector */
		double getLength() {
			double sum = 0;
			for (unsigned int i = 0; i < dimension; ++i) {
				sum += values[i] * values[i];
			}
			return sqrt(sum);
		}

		/** get a copy of this vector which is normalized (length = 1) */
		Vector getNormalized() {
			return *this / this->getLength();
		}

		/** normalize this vector (length = 1) */
		void normalize() {
			double length = getLength();
			for (unsigned int i = 0; i < dimension; ++i) {
				values[i] /= length;
			}
		}

		/** get the width of this vector (usually 1) */
		unsigned int getWidth() const {return 1;}

		/** get the height of this vector */
		unsigned int getHeight() const {return dimension;}

		/** add another vector to this one */
		Vector& operator += (const Vector& other) {
			for (unsigned int i = 0; i < dimension; ++i) {
				this->values[i] += other.values[i];
			}
			return *this;
		}

		/** subtract another vector from this one */
		Vector& operator -= (const Vector& other) {
			for (unsigned int i = 0; i < dimension; ++i) {
				this->values[i] -= other.values[i];
			}
			return *this;
		}

		/** multiply this vector with another one */
		Vector& operator *= (const Vector& other) {
			for (unsigned int i = 0; i < dimension; ++i) {
				this->values[i] *= other.values[i];
			}
			return *this;
		}

		/** divide this vector by another one */
		Vector& operator /= (const Vector& other) {
			for (unsigned int i = 0; i < dimension; ++i) {
				this->values[i] /= other.values[i];
			}
			return *this;
		}



		/** add another vector to this one and return result as copy */
		Vector operator + (const Vector& other) const {
			Vector copy = *this;
			return copy += other;
		}

		/** subtract another vector from this one and return result as copy */
		Vector operator - (const Vector& other) const {
			Vector copy = *this;
			return copy -= other;
		}

		/** multiply this vector with another one and return result as copy */
		Vector operator * (const Vector& other) const {
			Vector copy = *this;
			return copy *= other;
		}

		/** divide this vector by another one and return result as copy */
		Vector operator / (const Vector& other) const {
			Vector copy = *this;
			return copy /= other;
		}


		/** multiply vector by a constant */
		Vector& operator *= (const type other) {
			for (unsigned int i = 0; i < dimension; ++i) {
				values[i] *= other;
			}
			return *this;
		}

		/** divide vector by a constant */
		Vector& operator /= (const type other) {
			for (unsigned int i = 0; i < dimension; ++i) {
				values[i] /= other;
			}
			return *this;
		}


		/** multiply vector by a constant and return result as copy */
		Vector operator * (const type other) const {
			Vector copy = *this;
			return copy *= other;
		}

		/** divide vector by a constant and return result as copy */
		Vector operator / (const type other) const {
			Vector copy = *this;
			return copy /= other;
		}



		/** get a vector orthogonal to this one */
		Vector getOrthogonal() {
			static_assert(dimension == 2, "works only for 2-dimensional vectors");
			return {-values[1], values[0]};
		}



		/** check whether both vectors are equal */
		bool operator == (const Vector& other) const {
			bool same = true;
			for (unsigned int i = 0; i < dimension; ++i) {
				if (other.values[i] != values[i]) {
					same = false;
					break;
				}
			}
			return same;
		}

		/** array operator */
		type& operator[] (unsigned int idx) {
			return values[idx];
		}

		/** array operator */
		const type& operator[] (unsigned int idx) const {
			return values[idx];
		}


		/** get the underlying array */
		type* getArray() const {
			return values;
		}

		/** write to output stream */
		friend std::ostream& operator << (std::ostream& out, const Vector& v) {
			out << "vector(";
			out << v.values[0];
			for (unsigned int i = 1; i < dimension; ++i) {
				out << ", " << v.values[i];
			}
			out << ')';
			return out;
		}


	protected:

		/** store value for each coordinate */
		type values[dimension];
		//type* values;

	};

}

#endif // K_MATH_VECTOR_VECTOR_H_
