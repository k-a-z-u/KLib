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

		/** the random number generator */
		std::minstd_rand gen;
		bool isRandomized = false;

		/** X random indices */
		std::vector<int> indices;


	public:

		/** ctor */
		RandomIterator(const std::vector<Element>& vec, const int cnt) : vec(vec), cnt(cnt) {

			//const uint64_t ts = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			static int seed = 0; ++seed;
			gen.seed(seed);

			// sanity check
			if ((int)vec.size() < cnt)	{throw Exception("number of elements in list is smaller than the requested number to draw");}
			if (cnt == 0)				{throw Exception("number of elements in list must be at least 1");}
			if (vec.empty())			{throw Exception("empty input vector given");}

			indices.resize(cnt);

		}

		/** create random samples (vector-indicies) that are hereafter available for iteration */
		void randomize() {

			// random-number generator between [0:size-1]
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

			// the vector is setup correctly
			isRandomized = true;

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
			_assertTrue(isRandomized, "call randomize() before using the iterator!");
		}

	};

}

#endif // RANDOMITERATOR_H
