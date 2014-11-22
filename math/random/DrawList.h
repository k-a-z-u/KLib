#ifndef K_MATH_RANDOM_DRAWLIST_H
#define K_MATH_RANDOM_DRAWLIST_H

#include <vector>

namespace K {

	/**
	 * this class represents one entry within a DrawList.
	 * such an entry consists of userData denoted by the template argument
	 * and a probability.
	 */
	template <typename Entry> struct DrawListEntry {

	template <typename> friend class DrawList;
	friend class DrawList_Cumulative_Test;

	public:

		/** the user data behind this entry */
		Entry entry;

		/** this entry's probability */
		double probability;

	private:

		/** the cumulative probability, tracked among all entries within the DrawList */
		double cumulativeProbability;

	public:

		/** empty ctor */
		DrawListEntry() :
			entry(), probability(0), cumulativeProbability(0) {;}

		/** ctor */
		DrawListEntry(const Entry& entry, double probability) :
			entry(entry), probability(probability), cumulativeProbability(0) {;}

		/** compare this entrie's summed probability with the given probability */
		bool operator < (double probability) const {return cumulativeProbability < probability;}

	private:

		/** ctor */
		DrawListEntry(Entry& e, double probability, double summedProbability) :
			entry(entry), probability(probability), cumulativeProbability(summedProbability) {;}


	};



	/**
	 * a DrawList is a data-structure containing entries that have a
	 * probability assigned to them.
	 * using the draw() function one may draw from these entries according
	 * to their assigned probability in O(log(n))
	 */
	template <typename Entry> class DrawList {

	friend class DrawList_Cumulative_Test;

	public:

		/** ctor */
		DrawList() : sumValid(false) {;}


		/** append a new entry to the end of the list */
		void push_back(const Entry& entry, double probability) {
			const DrawListEntry<Entry> dle(entry, probability);
			entries.push_back(dle);
			sumValid = false;
		}

		/** change the entry at the given position. ensure the vector is resized()! */
		void set(uint32_t idx, const Entry& entry, double probability) {
			entries[idx].entry = entry;
			entries[idx].probability = probability;
			sumValid = false;
		}

		/** resize the underlying vector to hold the given number of entries */
		void resize(uint32_t numEntries) {entries.resize(numEntries);}

		/** clear all currently inserted entries */
		void clear() {entries.clear();}

		/** does the underlying vector contain any entries? */
		bool empty() {return entries.empty();}

		/** the number of entries */
		uint32_t size() {return entries.size();}

		/** draw a random entry from the draw list */
		Entry& draw() {

			// random value between [0, 1]
			double rand01 = double(rand()) / double(RAND_MAX);

			return draw(rand01);

		}

		/** draw an entry according to the given probability [0,1] */
		Entry& draw(double rand01) {

			// sanity check
			assert(!entries.empty());

			ensureCumulativeProbability();

			// random value between [0, summedProbability]
			// (this prevents us from norming the list to [0, 1])
			double rand = rand01 * entries[entries.size()-1].cumulativeProbability;

			// binary search for the matching entry O(log(n))
			auto tmp = std::lower_bound(entries.begin(), entries.end(), rand);
			return (*tmp).entry;

//			// O(n)
//			for (DrawListEntry<Entry>& dle : entries) {
//				if (dle.cumulativeProbability > rand) {return dle.entry;}
//			}
//			return entries[this->size()-1].entry;

		}

	private:

		/** ensure the cumulative probability is valid. if not -> calculate it */
		void ensureCumulativeProbability() {

			// already valid?
			if (sumValid) {return;}

			// calculate the cumulative probability
			double sum = 0;
			for (DrawListEntry<Entry>& dle : entries) {
				sum += dle.probability;
				dle.cumulativeProbability = sum;
			}

			// the sum is now valid
			sumValid = true;

		}

	private:

		/** all entries within the DrawList */
		std::vector<DrawListEntry<Entry>> entries;

		/** track wether the summedProbability is valid or not */
		bool sumValid;

	};




}

#endif // K_MATH_RANDOM_DRAWLIST_H
