#ifndef K_DATA_CASTITERATOR_H
#define K_DATA_CASTITERATOR_H

#include <vector>

/**
 * helper-class for std::vector iteration.
 * the returned iterator supplies only those entries in the original vector,
 * that are castable (dynamic_cast) to a requested subclass.
 */
template <typename BaseClass, typename SubClass> class CastIterator {

private:

	std::vector<BaseClass*>& vec;

private:

	class Iterator {

		/** current iteration index */
		int idx;

		/** vector of all instances */
		std::vector<BaseClass*>& vec;

	public:

		/** ctor */
		Iterator(const int idx, std::vector<BaseClass*>& vec) : idx(idx), vec(vec) {
			if (idx == -1) {inc();}
		}

		/** end-of-iteration operator */
		bool operator != (const Iterator& o) {return idx != o.idx;}

		/** instance access operator */
		SubClass* operator * () {
			BaseClass* base = vec[idx];
			return dynamic_cast<SubClass*>(base);
		}

		/** next element operator */
		Iterator& operator ++ () {
			inc();
			return *this;
		}

	private:

		/** increment idx until we found the next castable element */
		void inc() {
			while(true) {
				++idx;												// next element
				if (idx >= (int)vec.size()) {break;}				// reached end of data-vector?
				if (dynamic_cast<SubClass*>(vec[idx])) {break;}		// found a castable element?
			}
		}

	};


public:


	/** ctor */
	CastIterator(std::vector<BaseClass*>& vec) : vec(vec) {
		;
	}

	Iterator begin() {return Iterator(-1, vec);}

	Iterator end() {return Iterator(vec.size(), vec);}


};



#endif // K_DATA_CASTITERATOR_H
