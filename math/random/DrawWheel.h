#ifndef K_MATH_RANDOM_DRAWLIST_H
#define K_MATH_RANDOM_DRAWLIST_H

#include <vector>
#include "../distribution/Uniform.h"

/**
 * add entries to a list and be able to draw from them depending oh their probability
 *
 * souces:
 * https://www.udacity.com/course/viewer#!/c-cs373/l-48704330/e-48748082/m-48740082
 * https://www.youtube.com/watch?list=PLpUPoM7Rgzi_7YWn14Va2FODh7LzADBSm&feature=player_detailpage&v=kZhOJgooMxI#t=567
 */
namespace K {

	/**
	 * this class represents one entry within a DrawWheel.
	 * such an entry consists of userData denoted by the template argument
	 * and a probability.
	 */
	template <typename Entry> struct DrawWheelEntry {

	template <typename> friend class DrawWheel;

	public:

		/** the user data behind this entry */
		Entry entry;

		/** this entry's probability */
		double probability;

	public:

		/** empty ctor */
		DrawWheelEntry() : entry(), probability(0) {;}

		/** ctor */
		DrawWheelEntry(const Entry& entry, double probability) : entry(entry), probability(probability) {;}

	};

	/**
	 * a DrawWheel is a data-structure containing entries that have a
	 * probability assigned to them.
	 * using the draw() function one may draw from these entries according
	 * to their assigned probability in ~O(n)
	 */
	template <typename Entry> class DrawWheel {

	private:

		/** all entries within the DrawWheel */
		std::vector<DrawWheelEntry<Entry>> entries;

		/** is the current maximum valid? */
		bool maxValid = true;

		/** the maximum weight among all entries */
		double curMax = 0;

		/** the current index within the wheel */
		int curIdx = 0;

		/** the current offset at the wheel's index */
		double curOffset = 0;

		/** draw random numbers for the offset */
		K::UniformDistribution dist;


	public:

		/** ctor */
		DrawWheel() {;}


		/** append a new entry to the end of the list */
		void push_back(const Entry& entry, const double probability) {
			entries.push_back( DrawWheelEntry<Entry>(entry, probability) );
			if (curMax < probability) {curMax = probability;}
		}

		/** change the entry at the given position. ensure the vector is resized()! */
		void set(const uint32_t idx, const Entry& entry, const double probability) {
			entries[idx].entry = entry;
			entries[idx].probability = probability;
			maxValid = false;
		}

		/** resize the underlying vector to hold the given number of entries */
		void resize(const uint32_t numEntries) {entries.resize(numEntries);}

		/** clear all currently inserted entries */
		void clear() {entries.clear();}

		/** does the underlying vector contain any entries? */
		bool empty() const {return entries.empty();}

		/** the number of entries */
		uint32_t size() const {return entries.size();}


		/** call this once before drawing anything */
		void init() {

			// ensure the maximum number is correct
			ensureMaxProbability();

			// setup the distribution to draw a new offset
			dist.reset(0, 2 * curMax);

			// draw starting values
			curIdx = K::UniformDistribution::draw( (int)0, (int)entries.size() - 1);
			curOffset = dist.draw();

		}

		/** draw a random entry from the wheel */
		Entry& draw() {

			while(true) {

				// found a suitable particle? use it and draw the next random number
				if (entries[curIdx].probability >= curOffset) {

					// next offset
					curOffset += dist.draw();

					// return
					return entries[curIdx].entry;

				// weight to small, subtract the elements weight and move on to the next element
				} else {

					// remove the current entries probability
					curOffset -= entries[curIdx].probability;

					// resume with the next one along the wheel
					curIdx = (curIdx + 1) % ((int)entries.size());

				}

			}

		}



	private:

		/** ensure the max probability is valid. if not -> calculate it */
		void ensureMaxProbability() {

			// valid?
			if (maxValid) {return;}

			// comparisen
			const auto lambda = [] (const DrawWheelEntry<Entry>& e1, const DrawWheelEntry<Entry>& e2) {return e1.probability < e2.probability;};

			// find the largest entry
			const DrawWheelEntry<Entry> max = *std::max_element(entries.begin(), entries.end(), lambda);
			this->curMax = max.probability;

			// the max is now valid
			maxValid = true;

		}

	};

}

#endif // K_MATH_RANDOM_DRAWLIST_H
