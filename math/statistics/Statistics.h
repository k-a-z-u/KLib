/*
 * Statistics.h
 *
 *  Created on: Apr 23, 2013
 *      Author: Frank Ebner
 */

#ifndef K_MATH_STATISTICS_H_
#define K_MATH_STATISTICS_H_

#include <set>
#include <cstdint>

namespace K {

	/**
	 * store values here and get statistics about their
	 * avg, median, std-dev, etc.
	 */
	template <typename T> class Statistics {

	public:

		/** ctor */
		Statistics() : sum(0), sumSquared(0), cnt(0) {;}

		/** dtor */
		~Statistics() {;}

		/** add a new value */
		void add(T value) {
			++cnt;
			sum += value;
			sumSquared += (value*value);
			values.insert(value);
		}

		/** get the std-dev of all values */
		double getStdDev() const {
			double E1 = sumSquared / (double) cnt;
			double E2 = getAvg();
			return sqrt(E1 - (E2*E2));
		}

		/** get average value */
		double getAvg() const {
			return sum / (double) cnt;
		}

		/** get the given quantile (e.g. 0.5 for median) */
		double getQuantile(double q) const {
			if (q < 0 || q >= 1.0) {return -1;}
			uint32_t pos = cnt * q;
			uint32_t curPos = 0;
			for (auto val : values) {
				if (curPos == pos) {return val;}
				++curPos;
			}
			return -1;
		}

		/** get the median value (= Quantile 0.5) */
		double getMedian() const {
			return getQuantile(0.5f);
		}

		/** get smallest value */
		double getMin() const {
			if (values.empty()) {return -1;}
			return *(values.begin());
		}

		/** get largest value */
		double getMax() const {
			if (values.empty()) {return -1;}
			return *(--values.end());
		}

		/** get the squared sum */
		double getSquaredSum() const {
			return sumSquared;
		}


		/** get the sum of all values */
		T getSum() const {
			return sum;
		}

		/** get number of stored values */
		uint32_t getCount() const {
			return cnt;
		}

		/** get the number of values that are below "val" */
		T getNumValuesBelow(uint32_t val) const {
			uint32_t numFound = 0;
			for (auto curVal : values) {
				if (curVal > val) {return numFound;}
				++numFound;
			}
			return numFound;
		}

		/** send to stream */
		friend std::ostream& operator << (std::ostream& out, const Statistics<double>& s) {
			out << "min(" << s.getMin() << ")\t";
			out << "max(" << s.getMax() << ")\t";
			out << "med(" << s.getMedian() << ")\t";
			out << "avg(" << s.getAvg() << ")\t";
			out << "stdDev(" << s.getStdDev() << ")\t";
			out << "squared(" << s.getSquaredSum() << ")";
			return out;
		}

		/** reset all statistics */
		void reset() {
			sum = 0;
			sumSquared = 0;
			cnt = 0;
			values.clear();
		}

	private:

		/** sum of all added values */
		T sum;

		/** squared sum of all added values (for std-dev) */
		T sumSquared;

		/** the number of added values */
		uint32_t cnt;

		/** multiset to sort all values */
		std::multiset<T> values;

	};

}


#endif /* K_MATH_STATISTICS_H_ */
