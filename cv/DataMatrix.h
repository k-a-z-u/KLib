#ifndef K_CV_DATAMATRIX_H
#define K_CV_DATAMATRIX_H

#include <vector>
#include <iostream>

#include "../Assertions.h"

namespace K {

	/**
	 * matrix-like 2D data store
	 */
	template <typename T> class DataMatrix {

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

		/** dtor without data */
		DataMatrix(const int width, const int height) : width(width), height(height) {
			const int numElems = width*height;
			data.resize(numElems);
		}

		decltype(data.begin())	begin()	{return data.begin();}
		decltype(data.end())	end()	{return data.end();}



		/** get the image's width */
		int getWidth() const {return width;}

		/** get the image's height */
		int getHeight() const {return height;}

		/** get the underyling data array */
		T* getData() {return data.data();}


		/** get the value at (x,y) */
		T get(const int x, const int y) const {
			_assertBetween(x, 0, getWidth()-1, "x out of bounds: " + std::to_string(x));
			_assertBetween(y, 0, getHeight()-1, "y out of bounds: " + std::to_string(y));
			return data[x + y*width];
		}

		/** set the value at (x,y) */
		void set(const int x, const int y, const T v) {
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

	};

}

#endif // K_CV_DATAMATRIX_H

