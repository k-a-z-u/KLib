#ifndef GENETICLINEARTRANSFORMN_H
#define GENETICLINEARTRANSFORMN_H


#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "../../Assertions.h"
#include "../optimization/NumOptAlgoGenetic.h"

#include "../../misc/gnuplot/Gnuplot.h"
#include "../../misc/gnuplot/GnuplotPlot.h"
#include "../../misc/gnuplot/GnuplotPlotElementPoints.h"
#include "../../misc/gnuplot/GnuplotSplot.h"
#include "../../misc/gnuplot/GnuplotSplotElementPoints.h"

#include "../../math/distribution/NormalN.h"

#include <thread>
#include <chrono>

#include <eigen3/Eigen/Dense>

namespace K {

	/**
	 * Problem:
	 * - several classes
	 * - pattern-vectors [same size for each class]
	 * - training data [several pattern-vectors per class]
	 *
	 * the genetic linear transform will try to estimate a matrix [size MxN]
	 * to combine all input values and extract M features.
	 *
	 * this output value is "as distinct as possible" between all of the given classes
	 * using some optimization criteria (mahalanobis, ...)
	 */
	template <typename Scalar, typename ClassType, bool parallel = false> class GeneticLinearTransformN {

	public:

		/** input patterns are a vector containing several values */
		using Pattern = Eigen::VectorXf;

		/** the to-be-estimated matrix */
		using Matrix =  Eigen::MatrixXf;

		/** output feature is currently a single scalar value */
		using Feature = Eigen::VectorXf;

		/** classified pattern */
		using Classified = std::unordered_map<ClassType, std::vector<Pattern>>;

		/** helper-class to track stats */
		struct Stats {

			/** track all features */
			std::vector<Feature> features;

			bool init = false;
			int cnt = 0;
			Eigen::VectorXf sum;
			Eigen::MatrixXf sum2;


			void add(const Feature& f) {

				if (!init) {
					sum = f;
					sum2 = f * f.transpose();
					init = true;
				} else {
					sum += f;
					sum2 += f * f.transpose();
				}

				++cnt;

			}

			Eigen::MatrixXf getCovar() const {
				return (sum2/cnt) - ( (sum/cnt) * (sum/cnt).transpose() );
			}

			Eigen::VectorXf getAvgF() const {
				return sum/cnt;
			}

			Eigen::VectorXd getAvgD() const {
				return sum.cast<double>() / cnt;
			}

			/** get a normal-distribution that describes all contained samples */
			K::NormalDistributionN<float> getNormalDistribution() const {
				return K::NormalDistributionN<float> (getAvgF(), getCovar());
			}


		};

		struct PatternStats {
			std::vector<float> absSum;
			std::vector<float> sum;
			std::vector<float> sum2;
			std::vector<float> avg;
			std::vector<float> stdDev;
			std::vector<float> absAvg;
			std::vector<float> mod;
		};

	private:

		/** map containing all training patterns for each class */
		Classified classifiedPatterns;

		/** the to-be-estimated matrix */
		Matrix matrix;

		/** dimension of the feature-vectors to create */
		const int dimensions;

		/** length of one pattern [set once and used for sanity checks] */
		int patternLength = -1;

	public:

		/** ctor with the dimension to use for the output vector [usually 2-4] */
		GeneticLinearTransformN(const int dimensions) : dimensions(dimensions) {
			;
		}

		/** add a new training pattern for the given class */
		void addTrainingPattern(const ClassType clazz, const std::vector<Scalar>& pattern) {

			// wrap
			//Pattern map;
			//map.resize(pattern.size(), 1);
			//memcpy(map.data(), pattern.data(), pattern.size()*sizeof(Scalar));
			const Eigen::Map<Pattern> map((Scalar*)pattern.data(), pattern.size(), 1);

			// append
			addTrainingPattern(clazz, map);

		}

		/** add a new training pattern for the given class */
		void addTrainingPattern(const ClassType clazz, const Pattern& pattern) {

			// sanity check. same length for all patterns
			if (patternLength == -1) {patternLength = (int) pattern.rows();}
			_assertEqual(patternLength, pattern.size(), "all patterns must have the same length!");

			// append
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
		template <typename Func> void estimate(Func func, int numRuns = 200) {

			allocateMatrix();
			PatternStats stats = getStats();

			Scalar lastScore = -9999999;
			auto callback = [&] (const int x, const Scalar score, const Scalar* params) {
				std::cout << x << ": " << score << std::endl;
				if (score > lastScore) {
					debugView(params);
					lastScore = score;
				}
				//std::this_thread::sleep_for(std::chrono::milliseconds(1));
			};

			auto run = [&] (Scalar* data) {
				return -func(data, classifiedPatterns, patternLength, dimensions);
			};

			// number of parameters to optimize
			const int numParams = patternLength * dimensions;

			static std::minstd_rand gen;

			// prevent NaN issues etc.
			auto init = [&, numParams] (const int, Scalar* genes) {
				std::uniform_real_distribution<float> dist(-1.0f, +1.0f);
				for (int i = 0; i < numParams; ++i) {
					genes[i] = dist(gen);
				}
			};

			NumOptAlgoGenetic<Scalar, parallel> opt2(numParams);
			opt2.setPopulationSize(100);
			opt2.setMaxIterations(numRuns);
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

		/** get the feature output for the given pattern, using the trained matrix */
		const Feature getFeature(const std::vector<Scalar>& pattern) const {

			// wrap
			//Pattern map;
			//map.resize(pattern.size(), 1);
			//memcpy(map.data(), pattern.data(), pattern.size()*sizeof(Scalar));
			const Eigen::Map<Matrix> map((float*)pattern.data(), patternLength, 1);

			// get
			return getFeature(map);

		}

	private:

		/** allocate the to-be-estimated matrix and start with all ones (1.0) */
		void allocateMatrix() {
			const int w = patternLength;
			const int h = dimensions;
			matrix.resize(h, w);
			matrix.setOnes();
			//matrix.setZero(); // might yield some NaN issues and similar
		}

		/** multiply pattern by matrix to get the feature */
		static inline Feature mul(const Matrix& m, const Pattern& p) {
			return m * p;
		}

		/** wrap the given pointer data into an eigen-matrix */
		static inline Matrix wrap(const Scalar* s, const size_t patternLength, const size_t dimensions) {
//			Matrix mat;
//			mat.resize(dimensions, patternLength);
//			memcpy(mat.data(), s, patternLength*dimensions*sizeof(Scalar));
//			return mat;
			return Eigen::Map<Matrix>((float*)s, dimensions, patternLength);
		}


	public:

		Gnuplot gp;

		void debugView(const Scalar* params) {

			// one color per class
			const std::string colors[] = {"#000000", "#ff0000", "#00ff00", "#0000ff", "#ffff00"};

			GnuplotPlot plot;
			GnuplotPlotElementPoints points2[5];

			GnuplotSplot splot;
			GnuplotSplotElementPoints points3[5];

			// transformation matrix
			Matrix matrix = wrap(params, patternLength, dimensions);

			int classIdx = 0;
			for (const auto it : classifiedPatterns) {

				// 2D or 3D plot?
				if (dimensions < 3) {
					points2[classIdx].setColorHex(colors[classIdx % 5]);
					plot.add(&points2[classIdx]);
				} else {
					points3[classIdx].setColorHex(colors[classIdx % 5]);
					splot.add(&points3[classIdx]);
				}

				// process each pattern
				int idx2 = 0;
				for (const Pattern& p : it.second) {

					// convert each pattern to a feature vector
					const Feature f = matrix * p;

					// 1D, 2D or 3D?
					if (dimensions == 1) {
						const float offset = std::sin(idx2) * 0.1; ++idx2;
						//points2[classIdx].add(GnuplotPoint2(f[0], ((rand() % 25)*2+classIdx) / 100.0));//classIdx+offset));
						points2[classIdx].add(GnuplotPoint2(f[0], classIdx+offset));
					} else if (dimensions == 2) {
						points2[classIdx].add(GnuplotPoint2(f[0], f[1]));
					} else {
						points3[classIdx].add(GnuplotPoint3(f[0], f[1], f[2]));
					}

				}

				++classIdx;

			}

			// write to file
//			static int xxx = 0;
//			gp << "unset xtics\n";
//			gp << "unset ytics\n";
//			gp << "unset border\n";
//			gp << "set margins 0.1, 0.1, 0.1, 0.1\n";
//			gp.setTerminal("pngcairo", K::GnuplotSize(14, 3));
//			gp.setOutput("/tmp/2/opt_" + std::to_string(++xxx) + ".png");

			if (dimensions == 1) {
				gp << "set yrange [-1:" << (classIdx) << "]\n";
				gp.draw(plot);
			} else if (dimensions == 2) {
				gp.draw(plot);
			} else {
				gp.draw(splot);
			}



			gp.flush();

		}


	public:


		static float maximizeDistance(const Scalar* params, const Classified& classifiedPatterns, const int patternLength, const int dimensions) {

			const Matrix mat = wrap(params, patternLength, dimensions);
			std::unordered_map<ClassType, Stats> statsPerClass;

			for (const auto it : classifiedPatterns) {

				const ClassType clazz = it.first;

				// convert each pattern to a feature
				for (const Pattern& p : it.second) {
					const Feature f = mat * p;		// pattern -> feature
					statsPerClass[clazz].add(f);
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

					// distance between s1 and s2
					const float dist = (s1.getAvgF() - s2.getAvgF()).norm() / (s1.getCovar().norm() + s2.getCovar().norm());

					score += dist;

				}

			}

			return score;

		}



		static float bhattacharyya(const Scalar* params, const Classified& classifiedPatterns, const int patternLength, const int dimensions) {

			// https://en.wikipedia.org/wiki/Bhattacharyya_distance#cite_note-Coleman79-2

			std::vector<Stats> statsPerClass;
			const Matrix mat = wrap(params, patternLength, dimensions);

			// calculate features and stats for every class
			for (const auto it : classifiedPatterns) {

				Stats stats;

				// convert each pattern to a feature
				for (const Pattern& p : it.second) {
					const Feature f = mat*p;					// pattern -> feature
					stats.features.push_back(f);				// remember all features [prevent too many recalculations]
					stats.add(f);								// update stats
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
					const Eigen::MatrixXf combVar = (s1.getCovar() + s2.getCovar()) / 2.0;
					const Eigen::MatrixXf combVarInv = combVar.cast<double>().inverse().cast<float>();

					const double a =
						(s1.getAvgF() - s2.getAvgF()).transpose() *
						combVarInv *
						(s1.getAvgF() - s2.getAvgF());

					const double b =
						combVar.determinant() /
						std::sqrt(s1.getCovar().determinant() * s2.getCovar().determinant());

					const double x = a/8.0 + 0.5 * std::log(b);

					score += x;

					int zz = 0; (void) zz;

				}

			}

			return score;

		}

		static float mahalanobis(const Scalar* params, const Classified& classifiedPatterns, const int patternLength, const int dimensions) {

			std::vector<Stats> statsPerClass;

			const Matrix mat = wrap(params, patternLength, dimensions);

			// calculate features and stats for every class
			for (const auto it : classifiedPatterns) {

				Stats stats;

				// convert each pattern to a feature
				for (const Pattern& p : it.second) {
					const Feature f = mat*p;					// pattern -> feature
					stats.features.push_back(f);				// remember all features [prevent too many recalculations]
					stats.add(f);								// update stats
				}

				statsPerClass.push_back(stats);

			}

			// calculate be comparing each class with each other [SINGLE DIRECTION!]
			float score = 0;
			for (size_t i = 0; i < statsPerClass.size(); ++i) {
				for (size_t j = i+1; j < statsPerClass.size(); ++j) {

					const Stats& s1 = statsPerClass[j];
					const Stats& s2 = statsPerClass[i];


					// average covariance for both classes. correct?!?!
					// needs double otherwise inverse often fails to build
					const Eigen::MatrixXf combVarF = (s1.getCovar() + s2.getCovar()) / 2.0;
					const Eigen::MatrixXf combVarInvF = combVarF.cast<double>().inverse().cast<float>();
					//const Eigen::MatrixXd combVarInvD = combVarF.cast<double>().inverse();

					const Eigen::VectorXf diff = s1.getAvgF() - s2.getAvgF();

					const double x = diff.transpose() * combVarInvF * diff;

					// NaN e.g. due to univertible matrix, etc..
					if (x != x) {
						throw Exception("detected NaN");
					}

					score += std::sqrt(x);

				}

			}

			return score;

		}

		static float preventCrossMatch(const Scalar* params, const Classified& classifiedPatterns, const int patternLength, const int dimensions) {

			const Matrix mat = wrap(params, patternLength, dimensions);
			std::unordered_map<ClassType, Stats> statsPerClass;

			for (const auto it : classifiedPatterns) {

				const ClassType clazz = it.first;

				// convert each pattern to a feature
				for (const Pattern& p : it.second) {
					const Feature f = mat * p;		// pattern -> feature
					statsPerClass[clazz].add(f);
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

					const K::NormalDistributionN<float> nd1 = s1.getNormalDistribution();
					const K::NormalDistributionN<float> nd2 = s2.getNormalDistribution();

					const double p1 = nd1.getProbability(s2.getAvgF());
					const double p2 = nd2.getProbability(s1.getAvgF());
					const double d = (s1.getAvgF() - s2.getAvgF()).norm() / (s1.getCovar().norm() + s2.getCovar().norm());

					score += (1-p1) * (1-p2) * (d);

				}

			}

			return score;

		}

	};



}

#endif // GENETICLINEARTRANSFORMN_H
