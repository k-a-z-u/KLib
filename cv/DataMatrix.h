#ifndef K_CV_DATAMATRIX_H
#define K_CV_DATAMATRIX_H

#include <vector>
#include <iostream>
#include <functional>

#include "../Assertions.h"

namespace K {

	/**
	 * matrix-like 2D data store
	 */
	template <typename T> class DataMatrix {

	public:

		static T scalar;

	protected:

		/** the underlying data */
		std::vector<T> data;

		/** the data's width */
		int width;

		/** the data's height */
		int height;



	public:

		/** empty ctor */
		DataMatrix() : width(0), height(0) {
			;
		}

		/** ctor with data */
		DataMatrix(const T* data, const int width, const int height) : width(width), height(height) {
			const int numElems = width*height;
			this->data.insert(this->data.end(), data, data+numElems);
		}

		/** ctor without data */
		DataMatrix(const int width, const int height) : width(width), height(height) {
			const int numElems = width*height;
			data.resize(numElems);
		}

		/** get the data-size in bytes */
		size_t getSizeBytes() const {
			return data.size() * sizeof(T);
		}

//		/** no-copy */
//		DataMatrix(const DataMatrix& o) = delete;

//		/** move */
//		DataMatrix(const DataMatrix&& o) {
//			this->width = o.width;
//			this->height = o.height;
//			this->data = std::move(o.data);
//		}

//		DataMatrix& operator=(DataMatrix&& o) {
//			this->width = o.width;
//			this->height = o.height;
//			this->data = std::move(o.data);
//			return *this;
//		}

		decltype(data.begin())	begin()	{return data.begin();}
		decltype(data.end())	end()	{return data.end();}


		/** constant array access */
		const T operator () (const int x, const int y) const {return get(x,y);}


		/** get the image's width */
		inline int getWidth() const {return width;}

//		/** get the image's width */
//		inline int cols() const {return width;}

		/** get the image's height */
		inline int getHeight() const {return height;}

//		/** get the image's height */
//		inline int rows() const {return height;}

		/** get the underyling data array */
		T* getData() {return data.data();}

		/** get the underyling data array */
		inline const T* getData() const {return data.data();}

		/** get the value at (x,y) */
		inline T get(const int x, const int y) const {
			_assertBetween(x, 0, getWidth()-1, "x out of bounds: " + std::to_string(x));
			_assertBetween(y, 0, getHeight()-1, "y out of bounds: " + std::to_string(y));
			return data[x + y*width];
		}

		/** get the value at (x,y) */
		inline T& get(const int x, const int y) {
			_assertBetween(x, 0, getWidth()-1, "x out of bounds: " + std::to_string(x));
			_assertBetween(y, 0, getHeight()-1, "y out of bounds: " + std::to_string(y));
			return data[x + y*width];
		}

		/** set the value at (x,y) */
		inline void set(const int x, const int y, const T v) {
			_assertBetween(x, 0, getWidth()-1, "x out of bounds: " + std::to_string(x));
			_assertBetween(y, 0, getHeight()-1, "y out of bounds: " + std::to_string(y));
			data[x + y*width] = v;
		}

		/** set all entries to the given value */
		void setAll(const T v) {
			std::fill(data.begin(), data.end(), v);
		}

		/** debug output */
		friend std::ostream& operator << (std::ostream& out, const DataMatrix& m) {
			for (int y = 0; y < m.getHeight(); ++y) {
				for (int x = 0; x < m.getWidth(); ++x) {
					out << m.get(x,y) << '\t';
				}
				out << std::endl;
			}
			return out;
		}

		bool operator == (const DataMatrix& o) const {
			if (getWidth() != o.getWidth()) {return false;}
			if (getHeight() != o.getHeight()) {return false;}
			const int numElems = width*height;
			for (int i = 0; i < numElems; ++i) {
				if (data[i] != o.data[i]) {return false;}
			}
			return true;
		}

		/** call the given function for each of the channels's pixels.*/
		void forEach(std::function<void(const int, const int, const T)> exec) const {

			// run function for each element
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					exec(x, y, get(x,y));
				}
			}

		}

		/** add the given matrix */
		DataMatrix<T>& operator += (const DataMatrix<T>& o) {
			for (int i = 0; i < (int) data.size(); ++i) {
				data[i] += o.data[i];
			}
			return *this;
		}

		/** multiply the given matrix */
		DataMatrix<T>& operator *= (const DataMatrix<T>& o) {
			for (int i = 0; i < (int) data.size(); ++i) {
				data[i] *= o.data[i];
			}
			return *this;
		}

		/** multiply the given value */
		DataMatrix<T>& operator *= (const T val) {
			for (int i = 0; i < (int) data.size(); ++i) {
				data[i] *= val;
			}
			return *this;
		}

	};

}

#endif // K_CV_DATAMATRIX_H

