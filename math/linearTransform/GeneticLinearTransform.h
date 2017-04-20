#ifndef GENETICLINEARTRANSFORM_H
#define GENETICLINEARTRANSFORM_H

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "../../Assertions.h"
#include "../optimization/NumOptAlgoGenetic.h"

#include "../../misc/gnuplot/Gnuplot.h"
#include "../../misc/gnuplot/GnuplotPlot.h"
#include "../../misc/gnuplot/GnuplotPlotElementPoints.h"

#include "../../math/distribution/Normal.h"

#include <thread>
#include <chrono>

namespace K {

	/**
	 * Problem:
	 * - several classes
	 * - pattern-vectors [same size for each class]
	 * - training data [several pattern-vectors per class]
	 *
	 * the genetic linear transform will try to estimate a matrix [size 1xN]
	 * to combine all input values and extract one single feature.
	 *
	 * this output value is "as distinct as possible" between all of the given classes
	 * using some optimization criteria
	 */
	template <typename Scalar, typename ClassType, bool parallel = false> class GeneticLinearTransform {

	public:

		/** input patterns are a vector containing several values */
		using Pattern = std::vector<Scalar>;

		/** the to-be-estimated matrix */
		using Matrix = std::vector<Scalar>;

		/** output feature is currently a single scalar value */
		using Feature = Scalar;

		using Classified = std::unordered_map<ClassType, std::vector<Pattern>>;

		int patternLength = -1;

		struct Stats {

			int cnt = 0;
			double min = +999999999;
			double max = -999999999;
			double sum = 0;
			double sum2 = 0;
			std::vector<Feature> features;

			void add(const Scalar s) {
				if (s < min) {min = s;}
				if (s > max) {max = s;}
				sum += s;
				sum2 += s*s;
				++cnt;
			}

			inline double getRegion() const {return max - min;}

			inline double getAvg() const {return sum / (double)cnt;}

			inline double getAvg2() const {return sum2 / (double)cnt;}

			inline double getVar() const {return (getAvg2() - getAvg()*getAvg());}

			inline double getStdDev() const {return std::sqrt (getAvg2() - getAvg()*getAvg());}

			inline NormalDistribution getNormalDistribution() const {
				NormalDistribution nd(getAvg(), getStdDev());
				return nd;
			}

		};

	private:

		/** map containing all training patterns for each class */
		Classified classifiedPatterns;

		/** the to-be-estimated matrix */
		Matrix matrix;

	public:

		/** add a new training pattern */
		void addTrainingPattern(const ClassType clazz, const Pattern& pattern) {

			// sanity check. same length for all patterns
			if (patternLength == -1) {patternLength = (int) pattern.size();}
			_assertEqual(patternLength, pattern.size(), "all patterns must have the same length!");

			classifiedPatterns[clazz].push_back(pattern);

		}

		/** get a normal-distrbution matching the given class */
		Stats getStats(const ClassType clazz) {

			// get average and std-dev
			Stats stats;

			const std::vector<Pattern>& patterns = classifiedPatterns[clazz];
			for (const Pattern& p : patterns) {
				const Feature v = mul(matrix, p);
				stats.add(v);
			}

			return stats;

		}

		/** debug dump current training stats */
		void dumpTrainingStats() const {
			std::cout << "Training Data:\n";
			std::cout << "\tpattern size: " << patternLength << "\n";
			for (auto it : classifiedPatterns) {
				std::cout << "\tclass '" << it.first << "': " << it.second.size() << " patterns\n";
			}
			std::cout << std::endl;
		}



//		template <typename Func> void usedOnlyBestValues(Func func, const int num) {

//			// get a score for num "good" values of the pattern vector [contribution to the result]
//			bestIndices = getBestIndicies(num);

//			// keep only the good indices within the pattern vector and remove all others
//			for (auto& it : classifiedPatterns) {
//				std::vector<Pattern>& patterns = it.second;
//				for (Pattern& p : patterns) {
//					p = strip(p);
//				}
//			}

//			// we have reduced the pattern's dimension
//			patternLength = bestIndices.size();

//		}


		struct PatternStats {
			std::vector<float> absSum;
			std::vector<float> sum;
			std::vector<float> sum2;
			std::vector<float> avg;
			std::vector<float> stdDev;
			std::vector<float> absAvg;
			std::vector<float> mod;
		};

		/**
		 * calculate the average for each part [index] of the pattern
		 * among all classes. this can be used to determine the algorithm's
		 * starting parameters and usfull increment steps
		 */
		PatternStats getStats() {

			PatternStats res;
			res.sum.resize(patternLength);
			res.sum2.resize(patternLength);
			res.avg.resize(patternLength);
			res.mod.resize(patternLength);
			res.stdDev.resize(patternLength);
			res.absAvg.resize(patternLength);
			res.absSum.resize(patternLength);

			int numSamples = 0;
			for (const auto it : classifiedPatterns) {
				for (const Pattern& p : it.second) {
					++numSamples;
					for (size_t i = 0; i < patternLength; ++i) {
						res.absSum[i] += std::abs(p[i]);
						res.sum[i] += p[i];
						res.sum2[i] += p[i] * p[i];
					}
				}
			}

			for (size_t i = 0; i < patternLength; ++i) {
				res.absAvg[i] = res.absSum[i] / numSamples;
				res.avg[i] = res.sum[i] / numSamples;
				res.stdDev[i] = std::sqrt(res.sum2[i]/numSamples) - (res.avg[i]*res.avg[i]);
				res.mod[i] = res.stdDev[i] * 3;
			}

			return res;

		}

		/** estimate the linear transform */
		template <typename Func> void estimate(Func func) {

			allocateMatrix();
			PatternStats stats = getStats();

//			for (size_t i = 0; i < patternLength; ++i) {
//				matrix[i] = 1.0 / std::abs(stats.absAvg[i]);
//			}

			auto callback = [&] (const int x, const Scalar error, const Scalar* params) {
				std::cout << x << ": " << error << std::endl;
				debugView(params);
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			};

			auto run = [&] (const Scalar* data) {
				return -func(data, classifiedPatterns);
			};

			NumOptAlgoGenetic<Scalar, parallel> opt2(matrix.size());
			opt2.setPopulationSize(100);
			opt2.setMaxIterations(200);
			opt2.setValRange(0.25f);
//			opt2.setValRange(stats.mod);
			opt2.setElitism(0.15f);
			opt2.setMutation(0.15f);
			opt2.setCallback(callback);
			opt2.calculateOptimum(run, matrix.data());

		}

		/** get the estimated matrix */
		const Matrix& getMatrix() const {
			return matrix;
		}

		/** get the feature output for the given pattern, using the trained matrix */
		const Feature getFeature(const Pattern& p) const {
			return mul(matrix, p);
		}

	private:

//		struct ScoredEntry {
//			int idx;
//			float score;
//			ScoredEntry(const int idx, const float score) : idx(idx), score(score) {;}
//		};

//		std::vector<ScoredEntry> bestIndices;

//		Pattern strip(const Pattern& p) const {

//			if (bestIndices.empty()) {return p;}

//			Pattern res;
//			for (const ScoredEntry& e : bestIndices) {
//				res.push_back(p[e.idx]);
//			}
//			return res;

//		}

		/** allocate space for the to-be-estimated matrix */
		void allocateMatrix() {
			const int w = patternLength;
			const int h = 1;
			matrix.resize(w*h);
			std::fill(matrix.begin(), matrix.end(), 1.0);
		}

		/** multiply pattern by matrix to get the feature */
		static Feature mul(const Matrix& m, const Pattern& p) {
			return mul(m.data(), p.data(), m.size());
		}

		/** multiply pattern by matrix to get the feature */
		static Feature mul(const float* m, const float* p, const size_t cnt) {

			float sum = 0;
			for (size_t i = 0; i < cnt; ++i) {
				sum += m[i] * p[i];
			}

			_assertNotNAN(sum, "feature contains NaN");

			return sum;

		}

//		/**
//		 * the linear transform itself will just scale, shift and combine the values
//		 * from the pattern vector. thus, if one element of the vector is bad at all
//		 * [the values of all classes collide] the linear transform will not change this
//		 * and thus, this value does not perform well and can be kicked out.
//		 * this method will return the indices for the best "num" values of the pattern vector
//		 *
//		 */
//		std::vector<ScoredEntry> getBestIndicies(const int num) {

//			std::vector<ScoredEntry> entries;

//			for (int pat = 0; pat < patternLength; ++pat) {

//				auto func = [&] (const Pattern& p) {return p[pat];};
//				const float score = getDistance(func, classifiedPatterns);
//				entries.push_back(ScoredEntry(pat, score));

//			}

//			// sort by score. highest score first
//			auto comp = [] (const ScoredEntry& a, const ScoredEntry& b) {return a.score > b.score;};
//			std::sort(entries.begin(), entries.end(), comp);

//			entries.erase(entries.begin() + num, entries.end());

//			return entries;

//		}

	public:

		Gnuplot gp;

		void debugView(const Scalar* params) {

			GnuplotPlot plot;
			std::vector<GnuplotPlotElementPoints*> classes;

			std::string colors[] = {"#000000", "#ff0000", "#00ff00", "#0000ff", "#ffff00"};

			int idx = 0;
			for (const auto it : classifiedPatterns) {

				//const ClassType clazz = it.first;
				GnuplotPlotElementPoints* pts = new GnuplotPlotElementPoints();
				//pts->getColor().setHexStr(colors[idx % 5]);
				pts->setColorHex(colors[idx % 5]);
				plot.add(pts);

				// convert each pattern to a feature vector
				int idx2 = 0;
				for (const Pattern& p : it.second) {
					const Feature f = mul(params, p.data(), p.size());
					if (f == f) {
						const float offset = std::sin(idx2) * 0.1; ++idx2;
						pts->add(GnuplotPoint2(f, idx+offset));
					}
				}

				++idx;

			}

			gp << "set yrange [-1:" << (idx) << "]\n";

			gp.draw(plot);
			gp.flush();

		}


	public:

		static float maximizeDistance(const Scalar* params, const Classified& classifiedPatterns) {

			std::unordered_map<ClassType, Stats> statsPerClass;

			Stats all;

			for (const auto it : classifiedPatterns) {

				const ClassType clazz = it.first;

				// convert each pattern to a feature
				for (const Pattern& p : it.second) {
					const Feature f = mul(params, p.data(), p.size());			// pattern -> feature
					statsPerClass[clazz].add(f);
					all.add(f);
				}

			}

			std::unordered_set<std::string> analyzed;

			// calculate score
			float score = 0;
			for (const auto it1 : statsPerClass) {
				const ClassType clazz1 = it1.first;

				for (const auto it2 : statsPerClass) {
					const ClassType clazz2 = it2.first;

					if (clazz1 == clazz2) {continue;}

					const std::string hash = std::to_string(std::max(clazz1, clazz2)) + ":" + std::to_string(std::min(clazz1, clazz2));
					if (analyzed.find(hash) != analyzed.end()) {continue;}		// only check c1 -> c2. c2 -> c1 is the same
					analyzed.insert(hash);

					const Stats& s1 = it1.second;
					const Stats& s2 = it2.second;

					// s1 right of s2;
					const float fac = 2.2;
					const float d1 = (s1.getAvg() - s1.getStdDev()*fac) - (s2.getAvg() + s2.getStdDev()*fac);
					const float d2 = (s2.getAvg() - s2.getStdDev()*fac) - (s1.getAvg() + s1.getStdDev()*fac);
					score += std::max(d1,d2);

				}

			}

			return (score / all.getRegion());

		}



		static float bhattacharyya(const Scalar* params, const Classified& classifiedPatterns) {

			// https://en.wikipedia.org/wiki/Bhattacharyya_distance#cite_note-Coleman79-2

			std::vector<Stats> statsPerClass;

			// calculate features and stats for every class
			for (const auto it : classifiedPatterns) {

				Stats stats;

				// convert each pattern to a feature
				for (const Pattern& p : it.second) {
					const Feature f = mul(params, p.data(), p.size());		// pattern -> feature
					stats.features.push_back(f);							// remember all features [prevent too many recalculations]
					stats.add(f);											// update stats
				}

				statsPerClass.push_back(stats);

			}


			// calculate be comparing each class with each other [SINGLE DIRECTION!]
			float score = 0;
			for (size_t i = 0; i < statsPerClass.size(); ++i) {
				for (size_t j = i+1; j < statsPerClass.size(); ++j) {

					if (i == j) {continue;}
					const Stats& s1 = statsPerClass[j];
					const Stats& s2 = statsPerClass[i];

					// correct?
					const double combVar = (s1.getVar() + s2.getVar()) / 2.0;
					const double x =
							0.125 *
								(s1.getAvg() - s2.getAvg()) *
								(1.0 / combVar) *
								(s1.getAvg() - s2.getAvg())
							+
							0.5 * std::log(
								combVar /
								std::sqrt(s1.getVar() * s2.getVar())
							);

/*
					// get the normal distribution for both classes
					// into the formula, pass the patterns of BOTH classes
					// depending on the class the passed-in pattern belongs to
					// either one of the following probabilities p1/p2 gets almost 0.0
					// whichs

					// SLOWER!

					const K::NormalDistribution nd1 = s1.getNormalDistribution();
					const K::NormalDistribution nd2 = s2.getNormalDistribution();

					double sum = 0;
					for (const Feature f : s2.features) {
						const double p1 = nd1.getProbability(f);
						const double p2 = nd2.getProbability(f);
						sum += std::sqrt(p1*p2);
					}
					for (const Feature f : s1.features) {
						const double p1 = nd1.getProbability(f);
						const double p2 = nd2.getProbability(f);
						sum += std::sqrt(p1*p2);
					}

					// sum is ideally almost zero [because one probability is zero]
					//score += -std::log(sum);
					const double y = -std::log(sum);
*/

					score += x;

					int zz = 0; (void) zz;

				}

			}

			return score;

		}

		static float mahalanobis(const Scalar* params, const Classified& classifiedPatterns) {

			std::vector<Stats> statsPerClass;

			// calculate features and stats for every class
			for (const auto it : classifiedPatterns) {

				Stats stats;

				// convert each pattern to a feature
				for (const Pattern& p : it.second) {
					const Feature f = mul(params, p.data(), p.size());		// pattern -> feature
					stats.features.push_back(f);							// remember all features [prevent too many recalculations]
					stats.add(f);											// update stats
				}

				statsPerClass.push_back(stats);

			}

			// calculate be comparing each class with each other [SINGLE DIRECTION!]
			float score = 0;
			for (size_t i = 0; i < statsPerClass.size(); ++i) {
				for (size_t j = i+1; j < statsPerClass.size(); ++j) {

					const Stats& s1 = statsPerClass[j];
					const Stats& s2 = statsPerClass[i];

					// average covariance for both classes
					const double combVar = (s1.getVar() + s2.getVar()) / 2.0;

					const double x =
						(s1.getAvg() - s2.getAvg()) *
						(1.0 / combVar) *
						(s1.getAvg() - s2.getAvg());


					score += x;

				}

			}

			return score;

		}

		static float discreteSplit(const Scalar* params, const Classified& classifiedPatterns) {

			// calculate features and stats for every class
			std::unordered_map<ClassType, Stats> statsPerClass;
			for (const auto it : classifiedPatterns) {

				const ClassType clazz = it.first;

				// convert each pattern to a feature
				for (const Pattern& p : it.second) {
					const Feature f = mul(params, p.data(), p.size());			// pattern -> feature
					statsPerClass[clazz].features.push_back(f);					// remember all features [prevent too many recalculations]
					statsPerClass[clazz].add(f);								// update stats
				}

			}

			std::unordered_set<std::string> analyzed;

			// calculate score
			float score = 0;
			for (const auto it1 : statsPerClass) {
				const ClassType clazz1 = it1.first;

				for (const auto it2 : statsPerClass) {
					const ClassType clazz2 = it2.first;

					if (clazz1 == clazz2) {continue;}							// do not check class against itself

					const std::string hash = std::to_string(std::max(clazz1, clazz2)) + ":" + std::to_string(std::min(clazz1, clazz2));
					if (analyzed.find(hash) != analyzed.end()) {continue;}		// only check c1 -> c2. c2 -> c1 is the same
					analyzed.insert(hash);

					const Stats& s1 = it1.second;
					const Stats& s2 = it2.second;

					int numRight = 0;
					int numLeft = 0;

					// compare each feature of class2 with the min/max of all features of class1
					const float d = 0;//s1.getAvg() / 20.0f;
					for (const Feature f : s2.features) {
						if (f > s1.max + d) {++numRight;}
						if (f < s1.min - d) {++numLeft;}
					}

					// percentage of distinct values
					const float percent = (float) std::max(numRight, numLeft) / (float) s2.features.size();

					score += percent;

				}
			}

			const size_t numCompares = analyzed.size();
			return score / (float) numCompares;

		}

	};



}

#endif // GENETICLINEARTRANSFORM_H
