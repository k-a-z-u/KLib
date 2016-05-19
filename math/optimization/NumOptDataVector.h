#ifndef NUMOPTDATAVECTOR_H
#define NUMOPTDATAVECTOR_H

#include <vector>
#include "../../Assertions.h"

template <typename Scalar> class NumOptDataVector {

private:

	size_t size;
	Scalar* data = nullptr;

public:

	/** ctor */
	NumOptDataVector(const size_t size) : size(size), data(nullptr) {
		data = new Scalar[size];
		zero();
	}

	/** dtor */
	~NumOptDataVector() {
		delete data;
		data = nullptr;
	}

	/** copy */
	NumOptDataVector(const NumOptDataVector& o) : size(o.size) {
		data = new Scalar[o.size];
		memcpy(data, o.data, bytes());
	}

	/** move */
	NumOptDataVector(NumOptDataVector&& o) : size(o.size), data(o.data) {
		o.data = nullptr;
		o.size = -1;
	}

//	/** assign the given data-vector by copy */
//	void operator = (NumOptDataVector* o) {
//		_assertEqual(bytes(), o->bytes(), "size mismatch");
//		memcpy(data, o->data, bytes());
//	}

	/** assign the given data-vector by copy */
	NumOptDataVector& operator = (const NumOptDataVector& o) {
		if (size != o.size) {
			delete data;
			this->size = o.size;
			this->data = new Scalar[size];
		}
		memcpy(data, o.data, bytes());
		return *this;
	}

	void zero() {
		memset(data, 0, bytes());
	}

	/** size in bytes */
	inline size_t bytes() const {return size * sizeof(Scalar);}

	/** assign the given data-array by copy */
	void operator = (const Scalar* s) {
		memcpy(data, s, bytes());
		int i = 0; (void) i;
	}



	/** copy this vector into the given output */
	void copyTo(Scalar* dst) {
		memcpy(dst, data, bytes());
	}


	/** const array access */
	Scalar operator[] (const int idx) const {return data[idx];}

	/** array access */
	Scalar& operator[] (const int idx) {return data[idx];}


	/** const data access */
	const Scalar* constPtr() const {return data;}

	/** data access */
	Scalar* ptr() {return data;}

#define FOR_I for (unsigned int i = 0; i < size; ++i)


	NumOptDataVector& operator += (const Scalar value) {
		FOR_I {data[i] += value;}
		return *this;
	}

	NumOptDataVector& operator -= (const Scalar value) {
		FOR_I {data[i] -= value;}
		return *this;
	}

	NumOptDataVector& operator *= (const Scalar value) {
		FOR_I {data[i] *= value;}
		return *this;
	}

	NumOptDataVector& operator /= (const Scalar value) {
		FOR_I {data[i] /= value;}
		return *this;
	}


	NumOptDataVector operator + (const Scalar value) const {
		NumOptDataVector res(size);
		FOR_I {res.data[i] = data[i] + value;}
		return res;
	}

	NumOptDataVector operator - (const Scalar value) const {
		NumOptDataVector res(size);
		FOR_I {res.data[i] = data[i] - value;}
		return res;
	}

	NumOptDataVector operator * (const Scalar value) const {
		NumOptDataVector res(size);
		FOR_I {res.data[i] = data[i] * value;}
		return res;
	}

	NumOptDataVector operator / (const Scalar value) const {
		NumOptDataVector res(size);
		FOR_I {res.data[i] = data[i] / value;}
		return res;
	}



	NumOptDataVector& operator += (const NumOptDataVector& rhs) {
		FOR_I {data[i] += rhs.data[i];}
		return *this;
	}

	NumOptDataVector& operator -= (const NumOptDataVector& rhs) {
		FOR_I {data[i] -= rhs.data[i];}
		return *this;
	}



	NumOptDataVector operator + (const NumOptDataVector& rhs) const {
		NumOptDataVector res(size);
		FOR_I {res.data[i] = data[i] + rhs.data[i];}
		return res;
	}

	NumOptDataVector operator - (const NumOptDataVector& rhs) const {
		NumOptDataVector res(size);
		FOR_I {res.data[i] = data[i] - rhs.data[i];}
		return res;
	}



	/** get the length (euclid) of this vector */
	Scalar getLength() const {
		Scalar sum = 0;
		for (size_t i = 0; i < size; ++i) {sum += data[i] * data[i];}
		return std::sqrt(sum);
	}

#undef FOR_I



};

#endif // NUMOPTDATAVECTOR_H
