#ifndef GENETICLINEARTRANSFORM_H
#define GENETICLINEARTRANSFORM_H

#include <vector>
#include <unordered_map>

#include "../../Assertions.h"
#include "../optimization/NumOptAlgoGenetic.h"

#include "../../misc/gnuplot/Gnuplot.h"
#include "../../misc/gnuplot/GnuplotPlot.h"
#include "../../misc/gnuplot/GnuplotPlotElementPoints.h"

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
	template <typename Scalar, typename ClassType> class GeneticLinearTransform {

	public:

		/** input patterns are a vector containing several values */
		using Pattern = std::vector<Scalar>;

		/** the to-be-estimated matrix */
		using Matrix = std::vector<Scalar>;

		/** output feature is currently a single scalar value */
		using Feature = Scalar;

		using Classified = std::unordered_map<ClassType, std::vector<Pattern>>;

		int patternLength = -1;

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

		/** estimate the linear transform */
		template <typename Func> void estimate(Func func) {

			allocateMatrix();

			NumOptAlgoGenetic<Scalar> opt(matrix.size());
			opt.setPopulationSize(100);
			opt.setMaxIterations(100);
			opt.setValRange(1.0f);
			opt.setElitism(0.02f);
			opt.setMutation(0.40f);

			auto callback = [&] (const int x, const Scalar error, const Scalar* params) {
				std::cout << x << ": " << error << std::endl;
				debugView(params);
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			};
			opt.setCallback(callback);

			auto run = [&] (const Scalar* data) {
				return -func(data, classifiedPatterns);
			};
			opt.calculateOptimum(run, matrix.data());

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
			return sum;

		}

	public:

		void debugView(const Scalar* params) {

			static Gnuplot gp;
			GnuplotPlot plot;
			std::vector<GnuplotPlotElementPoints*> classes;

			for (const auto it : classifiedPatterns) {

				const ClassType clazz = it.first;
				GnuplotPlotElementPoints* pts = new GnuplotPlotElementPoints();
				plot.add(pts);

				// convert each pattern to a feature vector
				for (const Pattern& p : it.second) {
					const Feature f = mul(params, p.data(), p.size());
					pts->add(GnuplotPoint2(f, 0));
				}

			}

			gp.draw(plot);
			gp.flush();

		}


	public:

		static float maximizeDistance(const Scalar* params, const Classified& classifiedPatterns) {

			std::unordered_map<ClassType, Stats> statsPerClass;

			Stats all;

			for (const auto it : classifiedPatterns) {

				const ClassType clazz = it.first;

				// convert each pattern to a feature vector
				for (const Pattern& p : it.second) {
					const Feature f = mul(params, p.data(), p.size());
					statsPerClass[clazz].add(f);
					all.add(f);
				}

			}

			// calculate score
			float score = 0;
			for (const auto it1 : statsPerClass) {
				const ClassType clazz1 = it1.first;

				for (const auto it2 : statsPerClass) {
					const ClassType clazz2 = it2.first;

					if (clazz1 == clazz2) {continue;}

					const Stats& s1 = it1.second;
					const Stats& s2 = it2.second;

					// s1 right of s2;
					const float d1 = (s1.getAvg() - s1.getStdDev()) - (s2.getAvg() + s2.getStdDev());
					const float d2 = (s2.getAvg() - s2.getStdDev()) - (s1.getAvg() + s1.getStdDev());

					score += std::max(d1,d2);

				}

			}

			return score / all.getRegion();

		}

	private:

		struct Stats {

			int cnt = 0;
			Scalar min = +999999999;
			Scalar max = -999999999;
			Scalar sum;
			Scalar sum2;

			void add(const Scalar s) {
				if (s < min) {min = s;}
				if (s > max) {max = s;}
				sum += s;
				sum2 += s*s;
				++cnt;
			}

			Scalar getRegion() const {return max - min;}

			Scalar getAvg() const {return sum / cnt;}

			Scalar getAvg2() const {return sum2 / cnt;}

			Scalar getStdDev() const {return std::sqrt (getAvg2() - getAvg()*getAvg());}

		};

	};



}

#endif // GENETICLINEARTRANSFORM_H
