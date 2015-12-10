#ifndef K_MATH_STATISTICS_HISTOGRAM_H
#define K_MATH_STATISTICS_HISTOGRAM_H

#include "../../Assertions.h"

namespace K {

	class Histogram {

	private:

		double min;
		double max;
		double range;

		int numBins;

		std::vector<double> bins;

	public:

		/** ctor */
		Histogram(const double min, const double max, const int numBins) : min(min), max(max), range(max-min), numBins(numBins) {
			bins.resize(numBins);
		}

		/** add one new entry to the bin given by 'val' */
		void add(const double val) {
			++bins[getBin(val)];
		}

		/** add 'amount' to the bin denoted by 'val' */
		void add(const double val, const double amount) {
			bins[getBin(val)] += amount;
		}

		/** add the given amount to the given bin */
		void addToBin(const int bin, const double amount) {
			_assertBetween(bin, 0, numBins-1, "bin-index out of range");
			bins[bin] += amount;
		}

		/** get the value within the given bin */
		double getValue(const int bin) const {
			_assertBetween(bin, 0, numBins-1, "bin-index out of range");
			return bins[bin];
		}

		/** normalize the histogram (sum of 1.0) */
		void normalize() {
			const double sum = getSum();
			if (sum == 0) {return;}
			for (double& d : bins) {d /= sum;}
		}

		/** get the sum of all histogram bins */
		double getSum() const {
			double sum = 0;
			for (double d : bins) {sum += d;}
			return sum;
		}

		/** get the number of bins */
		int getNumBins() const {
			return numBins;
		}

		/** send to stream */
		friend std::ostream& operator << (std::ostream& out, const Histogram& h) {
			out << std::fixed;
			for (int bin = 0; bin < h.numBins; ++bin) {
				out << "["  << bin << "]:" << h.bins[bin] << " ";
			}
			return out;
		}




	private:

		/** get the bin for the given value */
		int getBin(const double val) const {

			// sanity check
			//_assertBetween(val, min, max, "value out of bounds: " + std::to_string(val));

			// value -> index
			int idx = (int) ( (val - min) / range * (numBins-1) );

			// sanity check
			_assertBetween(idx, 0, numBins-1, "index out of bounds: " + std::to_string(idx));

			return idx;

		}

	};

}

#endif // K_MATH_STATISTICS_HISTOGRAM_H

