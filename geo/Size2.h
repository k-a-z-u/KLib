#ifndef SIZE2_H
#define SIZE2_H

namespace K {

	/** 2D size */
	template <typename T> struct Size2 {

		/** x-coordinate */
		T w;

		/** y-coordinate */
		T h;

		/** empty ctor */
		Size2() : w(0), h(0) {;}

		/** ctor */
		Size2(const T w, const T h) : w(w), h(h) {;}


		/** equality check */
		bool operator == (const Size2& o) const {
			return (w == o.w) && (w == o.w);
		}

		Size2 operator * (const float val) const {return Size2(w*val, w*val);}


	};

	typedef Size2<float> Size2f;
	typedef Size2<int> Size2i;

}

#endif // SIZE2_H
