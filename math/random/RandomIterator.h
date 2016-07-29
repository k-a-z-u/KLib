#ifndef RANDOMITERATOR_H
#define RANDOMITERATOR_H

#include <vector>
#include <random>
#include "../../os/Time.h"

namespace K {

	template <typename Element> class RandomIterator {

	private:

		const std::vector<Element>& vec;

		int cnt;

		std::vector<int> indices;


	public:

		/** ctor */
		RandomIterator(const std::vector<Element>& vec, const int cnt) : vec(vec), cnt(cnt) {

			//static std::minstd_rand seedGen;
			//static std::uniform_int_distribution<int> seedDist(1337, 65535);
			//const int seed = seedDist(seedGen);

			static int seed = 1337; ++seed;

			//std::mt19937 gen(seed);
			std::minstd_rand gen(seed);
			std::uniform_int_distribution<int> dist(0, (int) vec.size()-1);
			bool used[vec.size()] = {false};

			for (int i = 0; i < cnt; ) {
				const int rnd = dist(gen);
				if (used[rnd]) {continue;}
				used[rnd] = true;
				++i;
				indices.push_back(rnd);
			}

		}

		struct Iterator {

			int pos;
			const std::vector<Element>& vec;
			const std::vector<int>& indices;

			Iterator(const int pos, const std::vector<Element>& vec, const std::vector<int>& indices) : pos(pos), vec(vec), indices(indices) {;}

			bool operator != (const Iterator& o) const {return pos != o.pos;}
			Iterator& operator ++ () {++pos; return *this;}
			Element operator * () {return vec[indices[pos]];}

		};

		const Element& operator [] (const int idx) const {return vec[indices[idx]]; }

		size_t size() const {return cnt;}

		Iterator begin() const { return Iterator(0, vec, indices); }
		Iterator end() const { return Iterator(cnt, vec, indices); }

	};

}

#endif // RANDOMITERATOR_H
