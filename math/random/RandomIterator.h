#ifndef RANDOMITERATOR_H
#define RANDOMITERATOR_H

#include <vector>
#include <random>
#include "../../Assertions.h"

namespace K {

	template <typename Element> class RandomIterator {

	private:

		/** the user's data-vector to randomly iterate */
		const std::vector<Element>& vec;

		/** the number of random indices */
		int cnt;


		/** the next seed */
		int seed = 0;

		/** X random indices */
		std::vector<int> indices;


	public:

		/** ctor */
		RandomIterator(const std::vector<Element>& vec, const int cnt) : vec(vec), cnt(cnt) {
			indices.resize(cnt);
		}

		/** create random samples (vector-indicies) that are hereafter available for iteration */
		void randomize() {

			// update the seed
			seed += 1337;

			// random-number generator between [0:size-1]
			std::minstd_rand gen(seed);
			std::uniform_int_distribution<int> dist(0, (int) vec.size()-1);

			// ensure we use each vector-index only ONCE
			bool used[vec.size()] = {false};

			// draw X random samples
			for (int i = 0; i < cnt; ) {
				const int rnd = dist(gen);
				if (used[rnd]) {continue;}		// already used? try again!
				used[rnd] = true;				// mark as used
				indices[i] = rnd;				// add to the index
				++i;
			}

		}

		/** the iterator state */
		struct Iterator {

			/** the current position within "indicies" */
			int pos;

			/** the vector with the user-data to randomly iterate */
			const std::vector<Element>& vec;

			/** the vector containing the random indices */
			const std::vector<int>& indices;

			/** ctor */
			Iterator(const int pos, const std::vector<Element>& vec, const std::vector<int>& indices) : pos(pos), vec(vec), indices(indices) {;}

			/** end-of-iteration? */
			bool operator != (const Iterator& o) const {return pos != o.pos;}

			/** next value */
			Iterator& operator ++ () {++pos; return *this;}

			/** get the user-data */
			Element operator * () {return vec[indices[pos]];}

		};

		//const Element& operator [] (const int idx) const {return vec[indices[idx]]; }

		/** number of available random entries */
		size_t size() const {return cnt;}


		/** for-each access */
		Iterator begin() const	{ ensureRandomized(); return Iterator(0, vec, indices); }

		/** for-each access */
		Iterator end() const	{ ensureRandomized(); return Iterator(cnt, vec, indices); }

private:

		/** ensure the coder called randomize() before using the iterator */
		void ensureRandomized() const {
			_assertNot0(seed, "call randomize() before using the iterator!");
		}

	};

}

#endif // RANDOMITERATOR_H
