#ifndef K_MATH_STATISTICS_HISTOGRAM_H
#define K_MATH_STATISTICS_HISTOGRAM_H

#include "../../Assertions.h"

#include <vector>

namespace K {

	template <typename T> class Histogram {

		T min;
		T max;
		T range;
		T binSize;
		int numBins;

		std::vector<T> bins;

	public:

		/** empty ctor */
		Histogram() : min(0), max(0), range(0), binSize(0), numBins(0) {
			;
		}

		/** ctor */
		Histogram(const T min, const T max, const int numBins) : min(min), max(max), range(max-min), binSize(range/numBins), numBins(numBins) {
			bins.resize(numBins);
		}


		float getMin() const {return min;}

		float getMax() const {return max;}

		/** get the size of one bin */
		float getBinSize() const {
			return binSize;
		}

		/** get the number of bins */
		int getNumBins() const {
			return numBins;
		}

		/** get the center-value for the given bin */
		float getCenter(const int bin) const {
			_assertBetween(bin, 0, numBins-1, "bin-index out of range: " + std::to_string(bin));
			return min + (range / float(numBins) * (float(bin) + 0.5f));
		}

		/** add the given "amount" to the bin best matching "val" */
		void add(const float val, const T amount) {
			bins[getBin(val)] += amount;
		}

		/** add the given "amount" to the two bins best matching "val" */
		void addInterpolate(const float val, const T amount) {

			// sanity check
			_assertBetween(val, min, max, "value out of range: " + std::to_string(val));

			// get the two bins best matching the given value
			int idx1 = (int) std::floor((val-binSize/2)/range*float(numBins));		// binSize/2 = move to the bin's center
			int idx2 = (int)  std::ceil((val-binSize/2)/range*float(numBins));		// binSize/2 = move to the bin's center

			// ensure we are within the range
			if (idx1 < 0)			{idx1 = 0;}
			if (idx2 >= numBins)	{idx2 = numBins-1;}

			// interpolate?
			if (idx1 == idx2) {

				// no interpolation necessary!
				bins[idx1] += amount;

			} else {

				// interpolate the result by adding the "amount" shared between those two bins
				const float v1 = std::abs(val - getCenter(idx2)) / binSize;			// distance from bin-2
				const float v2 = std::abs(val - getCenter(idx1)) / binSize;			// distance from bin-1

				// add
				bins[idx1] += amount * v1;
				bins[idx2] += amount * v2;

			}

		}

		/** set the given bin to the provided value */
		void set(const int bin, const T val) {
			bins[bin] = val;
		}

		/** get the value for the given bin */
		T get(const int bin) const {
			_assertBetween(bin, 0, numBins-1, "bin-index out of range: " + std::to_string(bin));
			return bins[bin];
		}

		/** get the bin the given value belongs to */
		int getBin(const T val) {

			// sanity check
			_assertBetween(val, min, max, "value out of range: " + std::to_string(val));

			// calculate
			return std::round((val-binSize/2)/range*float(numBins));				// binSize/2 = move to the bin's center

		}

		/** set all bins to zero */
		void zero() {
			std::fill (bins.begin(), bins.end(), 0.0f);
		}

		/** normalize the histogram (sum of 1.0) */
		void normalize() {
			const double sum = getSum();
			if (sum == 0) {return;}
			for (T& d : bins) {d /= sum;}
		}

		/** get the sum of all histogram bins */
		T getSum() const {
			double sum = 0;
			for (T d : bins) {sum += d;}
			return sum;
		}

		/** get the data from the contained bins */
		const T* getData() const {
			return bins.data();
		}

		/** get the underlying data vector */
		const std::vector<float>& getVector() const {
			return bins;
		}

		/** send to stream */
		friend std::ostream& operator << (std::ostream& out, const Histogram& h) {
			out << std::fixed;
			for (int bin = 0; bin < h.numBins; ++bin) {
				out << "["  << bin << "]:" << h.bins[bin] << " ";
			}
			return out;
		}



	};

}

#endif // K_MATH_STATISTICS_HISTOGRAM_H

