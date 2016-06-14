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
	NumOptDataVector() : size(0), data(nullptr) {
		;
	}

	/** ctor */
	NumOptDataVector(const size_t size) {
		resize(size);
		zero();
	}

	/** ctor with data */
	NumOptDataVector(const size_t size, const Scalar* data) {
		resize(size);
		copyFrom(data);
	}

	/** copy ctor */
	NumOptDataVector(const NumOptDataVector& o) : size(o.size) {
		if (!o.data) {
			throw "other data is null!";
		}
		resize(o.size);
		memcpy(data, o.data, bytes());
	}

	/** move ctor */
	NumOptDataVector(NumOptDataVector&& o) : size(o.size), data(o.data) {
		o.data = nullptr;
		o.size = -1;
	}

	/** dtor */
	~NumOptDataVector() {
		free();
	}


	/** assign the given data-vector by copy */
	NumOptDataVector& operator = (const NumOptDataVector& o) {
		if (size != o.size) { resize(o.size); }
		memcpy(data, o.data, bytes());
		return *this;
	}

	/** move assign */
	NumOptDataVector& operator = (NumOptDataVector&& o) {
		free();
		this->data = o.data; o.data = nullptr;
		this->size = o.size; o.size = -1;
		return *this;
	}



//	/** assign the given data-vector by copy */
//	void operator = (NumOptDataVector* o) {
//		_assertEqual(bytes(), o->bytes(), "size mismatch");
//		memcpy(data, o->data, bytes());
//	}



	void zero() {
		memset(data, 0, bytes());
	}

	/** size in bytes */
	inline size_t bytes() const {return size * sizeof(Scalar);}

	/** number of entries */
	inline size_t getSize() const {return size;}

	/** assign the given data-array by copy */
	void operator = (const Scalar* s) {
		memcpy(data, s, bytes());
	}



	/** copy this vector into the given output */
	void copyTo(Scalar* dst) {
		memcpy(dst, data, bytes());
	}

	/** copy the given data into this vector */
	void copyFrom(const Scalar* src) {
		memcpy(data, src, bytes());
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


#undef FOR_I



	/** get the length (euclid) of this vector */
	Scalar getLength() const {
		Scalar sum = 0;
		for (size_t i = 0; i < size; ++i) {sum += data[i] * data[i];}
		return std::sqrt(sum);
	}

private:

	void free() {
		::free(data);
		data = nullptr;
	}

	void resize(const size_t size) {
		free();
		this->size = size;
		this->data = (Scalar*) malloc(bytes());
	}




};

#endif // NUMOPTDATAVECTOR_H
