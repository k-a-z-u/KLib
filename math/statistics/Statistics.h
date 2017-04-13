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
#include <sstream>
#include <cmath>

namespace K {

	/**
	 * store values here and get statistics about their
	 * avg, median, std-dev, etc.
	 */
	template <typename T> class Statistics {

	public:

		/** ctor */
		Statistics() : sum(), sumSquared(), cnt() {;}

		/** dtor */
		~Statistics() {;}

		/** add a new value */
		void add(T value) {
			++cnt;
			sum += value;
			sumSquared += (value*value);
			values.insert(value);
		}

		/** add all values from the given statistics instance */
		void add(const K::Statistics<T>& other) {
			for (const T val : other.values) {
				this->add(val);
			}
		}

		/** get the std-dev of all values */
		T getStdDev() const {
			double E1 = sumSquared / (double) cnt;
			double E2 = getAvg();
			return std::sqrt(E1 - (E2*E2));
		}

		/** get average value */
		T getAvg() const {
			return sum / (double) cnt;
		}

		/** get the given quantile (e.g. 0.5 for median) */
		T getQuantile(const double q) const {
			if (q < 0) {return *values.begin();}
			if (q >= 1) {return *(--values.end());}
			uint32_t pos = cnt * q;
			uint32_t curPos = 0;
			for (auto val : values) {
				if (curPos == pos) {return val;}
				++curPos;
			}
			return -1;
		}

		/** get the median value (= Quantile 0.5) */
		T getMedian() const {
			return getQuantile(0.5f);
		}

		/** get smallest value */
		T getMin() const {
			if (values.empty()) {return -1;}
			return *(values.begin());
		}

		/** get largest value */
		T getMax() const {
			if (values.empty()) {return -1;}
			return *(--values.end());
		}

		/** get the range between min an max */
		T getRange() const {
			return getMax() - getMin();
		}

		/** get the squared sum */
		T getSquaredSum() const {
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

		/** get as string */
		std::string asString() const {
			std::stringstream ss;
			appendTo(ss);
			return ss.str();
		}

		/** send to the given stream */
		void appendTo(std::ostream& out) const {
			out.precision(6);
			out.setf( std::ios::fixed, std:: ios::floatfield );
			out << "cnt(" << getCount() << ")\t";
			out << "min(" << getMin() << ")\t";
			out << "max(" << getMax() << ")\t";
			out << "range(" << getRange() << ")\t";
			out << "med(" << getMedian() << ")\t";
			out << "avg(" << getAvg() << ")\t";
			out << "stdDev(" << getStdDev() << ")\t";
		}

		/** send to stream */
		inline std::ostream& operator << (std::ostream& out) const {
			appendTo(out); return out;
		}

		/** reset all statistics */
		void reset() {
			sum = T();
			sumSquared = T();
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
