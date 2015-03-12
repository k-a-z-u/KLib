#ifndef TESTRANSAC_CPP
#define TESTRANSAC_CPP

#ifdef WITH_TESTS

#include "../../../Test.h"
#include "../../../../math/filter/ransac/Ransac.h"
#include <vector>
#include "../../../../os/Process.h"
#include "../../../../misc/gnuplot/Gnuplot.h"

namespace K {

	struct MyRansacSample {
		double x;
		double y;
		MyRansacSample(double x, double y) : x(x), y(y) {;}
	};

	class MyRansacSampleSet : public RansacSampleSet<MyRansacSample> {

	public:

		std::vector<const MyRansacSample*> samples;

	public:

		virtual uint32_t getSize() const override {return samples.size();}

		virtual const MyRansacSample* getSample(const int idx) const override {return samples[idx];}

		virtual void remove(const MyRansacSample* s) {
			samples.erase(std::remove(samples.begin(), samples.end(), s), samples.end());
		}

	};

	class MyRansacSelector : public RansacSelector<MyRansacSample> {
	public:
		virtual RansacSampleSet<MyRansacSample>* getRandomSubset(const RansacSampleSet<MyRansacSample>& input) override {
			MyRansacSampleSet* out = new MyRansacSampleSet();
			for (int i = 0; i < 2; ++i) {
				int idx = rand() % input.getSize();
				const MyRansacSample* sample = input.getSample(idx);
				out->samples.push_back(sample);
			}
			return out;
		}

	};

	class MyRansacModelParams : public RansacModelParameters<2> {

	};

	class MyRansacEstimator : public RansacEstimator<MyRansacSample, 2> {
	private:
		Gnuplot gp;
	public:
		virtual MyRansacModelParams* getEstimatedModelParameters(const RansacSampleSet<MyRansacSample>& subset) override {

			MyRansacModelParams* params = new MyRansacModelParams();

			const MyRansacSample* s1 = subset.getSample(0);
			const MyRansacSample* s2 = subset.getSample(1);

			const double m = (s2->y - s1->y) / (s2->x - s1->x);
			const double t = (s1->y - m * s1->x);
			(*params)[1] = m;
			(*params)[0] = t;

			gp << "set xrange [-200:1200]\n";
			gp << "set yrange [-200:1200]\n";
			gp << "plot ";
			gp << "'-' with points\n";
			gp << s1->x << " " << s1->y << "\n";
			gp << s2->x << " " << s2->y << "\n";
			gp << "e\n";
			gp.flush();

			return params;


		}
	};

	class MyRansacConsensus : public RansacConsensus<MyRansacSample, 2> {
	private:
		Gnuplot gp;
	public:
		MyRansacSampleSet* getConsensusSet(const RansacSampleSet<MyRansacSample>* allSamples, RansacModelParameters<2>& params) override {
			MyRansacSampleSet* input = (MyRansacSampleSet*) allSamples;
			MyRansacSampleSet* consensus = new MyRansacSampleSet();

			gp.setDebugOutput(false);
			gp << "set xrange [-200:1200]\n";
			gp << "set yrange [-200:1200]\n";

			gp << "plot ";
			gp << "'-' with lines,";
			gp << "'-' with points,";
			gp << "'-' with points ";
			gp << "\n";

			double x1 = -100;
			double x2 = 1100;
			double y1 = x1 * params[1] + params[0];
			double y2 = x2 * params[1] + params[0];
			gp << x1 << " " << y1 << "\n";
			gp << x2 << " " << y2 << "\n";
			gp << "e\n";

			for (const MyRansacSample* s : input->samples) {
				double t = params[0];
				double m = params[1];
				double y = (m * s->x) + t;
				double diff = std::abs(s->y - y);
				if (diff < 50) {
					consensus->samples.push_back(s);
				} else {
					gp << s->x << " " << s->y << "\n";
				}

			}

			gp << "e\n";

			for (const MyRansacSample* s : consensus->samples) {
				gp << s->x << " " << s->y << "\n";
			}
			gp << "e\n";


			gp.flush();

						usleep(1000 * 1000);


			return consensus;
		}
//		MyRansacSampleSet* getConsensusSet(MyRansacSampleSet* allSamples, RansacModelParameters<2>* params) override {
//			MyRansacSampleSet* consensus = new MyRansacSampleSet();
//			for (const MyRansacSample* s : allSamples->samples) {
//				double t = params[0];
//				double m = params[1];
//				double y = m * s->x + t;
//				double diff = std::abs(s->y - y);
//				if (diff < 25) {
//					consensus->samples.push_back(s);
//				}
//			}
//			return consensus;
//		}
	};

	TEST(Ransac, line) {

		MyRansacSampleSet set;
		MyRansacSelector sel;
		MyRansacEstimator est;
		MyRansacConsensus cons;

		// genrated good samples with only a slight error
		for (int i = 0; i < 1024; ++i) {
			int x = rand() % 1024;
			int y = x + ((rand() % 128) - 64);
			set.samples.push_back(new MyRansacSample(x,y));
		}

		// generate same samples with a very bad error
		for (int i = 0; i < 128; ++i) {
			int x = rand() % 1024;
			int y = rand() % 1024;
			set.samples.push_back(new MyRansacSample(x,y));
		}

		Ransac<MyRansacSample, 2> ransac;
		ransac.getConsensus(set, &sel, &est, &cons);


	}

}

#endif // WITH_TESTS

#endif // TESTRANSAC_CPP
