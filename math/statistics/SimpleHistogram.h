#ifndef K_SIMPLEHISTOGRAM_H
#define K_SIMPLEHISTOGRAM_H

#include <map>

namespace K {

	class SimpleHistogram {

		const float binSize;

		/** each bin gets an integer number */
		std::map<int, float> bins;

	public:

		/** ctor with bin-size */
		SimpleHistogram(const float binSize) : binSize(binSize) {
			;
		}

		/** add "y" entries to the bin that "x" belongs to */
		void add(const float x, const float y) {
			const int binNr = xToBin(x);
			bins[binNr] += y;
		}

		/** ensure sum is 1.0 */
		void normalize() {
			float sum = 0;
			for (const auto& it : bins) { sum += it.second; }
			sum *= binSize;
			for (auto& it : bins) {it.second /= sum;}
		}

		/** get a list of (binCenter, amount) entries where missing bin's are added as zero-amount */
		std::vector<std::pair<float,float>> getFilled() const {
			const int minBin = bins.begin()->first;
			const int maxBin = (--bins.end())->first;
			std::vector<std::pair<float, float>> res;
			for (int i = minBin; i <= maxBin; ++i) {
				const float x = binToX(i);
				const float y = (bins.find(i) != bins.end()) ? (bins.find(i)->second) : (0);
				res.push_back(std::pair<float,float>(x, y));
			}
			return res;
		}

		/** get the first bin's x value */
		float getMinX() const {
			return binToX(bins.begin()->first);
		}

		/** get the last bin's x value */
		float getMaxX() const {
			return binToX((bins.end()--)->first);
		}

	private:

		/** convert from x to bin-number */
		int xToBin(const float x) const {
			return std::round(x/binSize);
		}

		/** convert from bin-number to x */
		float binToX(const int bin) const {
			return bin * binSize;
		}

	};

}

#endif // K_SIMPLEHISTOGRAM_H
