
#ifdef WITH_TESTS

#include "../../Test.h"

#include "../../../math/filter/particles/Particle.h"
#include "../../../math/filter/particles/ParticleFilter.h"

#include "../../../math/filter/particles/estimation/ParticleFilterEstimationMax.h"
#include "../../../math/filter/particles/estimation/ParticleFilterEstimationWeightedAverage.h"
#include "../../../math/filter/particles/estimation/ParticleFilterEstimationRegionalWeightedAverage.h"

#include "../../../math/filter/particles/resampling/ParticleFilterResamplingSimple.h"
#include "../../../math/filter/particles/resampling/ParticleFilterResamplingNone.h"

#include "../../../math/distribution/Normal.h"

namespace K {

	struct MyState {
		double x;
		double y;
		MyState() : x(0), y(0) {;}
		MyState(double x, double y) : x(x), y(y) {;}
		MyState& operator += (const MyState& other) {
			x += other.x;
			y += other.y;
			return *this;
		}
		MyState& operator /= (double d) {
			x /= d;
			y /= d;
			return *this;
		}
		MyState operator * (double d) const {
			return MyState(x*d, y*d);
		}
		MyState& operator = (const MyState& other) {
			this->x = other.x;
			this->y = other.y;
			return *this;
		}
		double distance(const MyState& o) const {
			return std::sqrt( (x-o.x)*(x-o.x) + (y-o.y)*(y-o.y) ) / 4.9;
		}
		bool belongsToRegion(const MyState& o) const {
			return distance(o) <= 1.0;
		}

	};

	struct MyObservation {
		double x;
		double y;
		MyObservation() : x(0), y(0) {;}
		void set(double x, double y) {this->x = x; this->y = y;}
	};

	TEST(Particles, estimationMax) {

		std::vector<Particle<MyState>> vec;
		vec.push_back(Particle<MyState>(MyState(0,1), 0.1));
		vec.push_back(Particle<MyState>(MyState(0,2), 0.2));
		vec.push_back(Particle<MyState>(MyState(0,3), 0.3));
		vec.push_back(Particle<MyState>(MyState(0,4), 0.2));

		ParticleFilterEstimationMax<MyState> max;
		MyState s = max.estimate(vec);

		ASSERT_EQ(3, s.y);

	}

	TEST(Particles, estimationAvg) {

		std::vector<Particle<MyState>> vec;
		vec.push_back(Particle<MyState>(MyState(0,1), 0.1));
		vec.push_back(Particle<MyState>(MyState(0,2), 0.2));
		vec.push_back(Particle<MyState>(MyState(0,3), 0.3));
		vec.push_back(Particle<MyState>(MyState(0,4), 0.2));

		ParticleFilterEstimationWeightedAverage<MyState> avg;
		MyState s = avg.estimate(vec);

		ASSERT_EQ(0.00, s.x);
		ASSERT_EQ(2.75, s.y);

	}

	TEST(Particles, estimationRegAvg) {

		std::vector<Particle<MyState>> vec;
		vec.push_back(Particle<MyState>(MyState(0,0), 0.5));	// << most probable
		vec.push_back(Particle<MyState>(MyState(0,1), 0.1));
		vec.push_back(Particle<MyState>(MyState(0,2), 0.2));
		vec.push_back(Particle<MyState>(MyState(0,3), 0.1));
		vec.push_back(Particle<MyState>(MyState(0,4), 0.2));

		vec.push_back(Particle<MyState>(MyState(0,5), 0.4));	// will not be used due to distance()
		vec.push_back(Particle<MyState>(MyState(0,6), 0.1));
		vec.push_back(Particle<MyState>(MyState(0,7), 0.2));
		vec.push_back(Particle<MyState>(MyState(0,8), 0.1));
		vec.push_back(Particle<MyState>(MyState(0,9), 0.2));

		// check the used distance metric
		ASSERT_GE(1.0, vec[0].state.distance(vec[1].state));
		ASSERT_GE(1.0, vec[0].state.distance(vec[2].state));
		ASSERT_GE(1.0, vec[0].state.distance(vec[3].state));
		ASSERT_GE(1.0, vec[0].state.distance(vec[4].state));

		ASSERT_LE(1.0, vec[0].state.distance(vec[5].state));
		ASSERT_LE(1.0, vec[0].state.distance(vec[6].state));
		ASSERT_LE(1.0, vec[0].state.distance(vec[7].state));
		ASSERT_LE(1.0, vec[0].state.distance(vec[8].state));
		ASSERT_LE(1.0, vec[0].state.distance(vec[9].state));

		// estimate
		ParticleFilterEstimationRegionalWeightedAverage<MyState> avg;
		MyState s = avg.estimate(vec);

		// check the resulting values
		ASSERT_FLOAT_EQ(0, s.x);
		ASSERT_FLOAT_EQ(1.6/1.1, s.y);

	}


	TEST(Particles, resample) {

		unsigned int cnt = 1000;

		std::vector<Particle<MyState>> vec;
		vec.push_back(Particle<MyState>(MyState(0,1), 0.1));
		vec.push_back(Particle<MyState>(MyState(0,2), 0.3));
		vec.push_back(Particle<MyState>(MyState(0,3), 0.6));
		for (unsigned int i = 0; i < cnt-3; ++i) {
			vec.push_back(Particle<MyState>(MyState(0,4), 0.00000001));
		}

		ParticleFilterResamplingSimple<MyState> res;
		int check[4] = {0};

		// resample and count number of resampled particles
		res.resample(vec);
		for (Particle<MyState>& p : vec) {
			++check[(int) p.state.y - 1];
			p.weight = 1;						// reset weight for the next run
		}

		//std::cout << check[0] << ":" <<check[1] << ":" << check[2] << ":" << check[3] << std::endl;

		// check particle distribution
		ASSERT_NEAR(0.1*cnt, check[0], 20);
		ASSERT_NEAR(0.3*cnt, check[1], 15);
		ASSERT_NEAR(0.6*cnt, check[2], 15);
		ASSERT_NEAR(0.0*cnt, check[3], 2);

		// resample and count number of resampled particles
		check[0] = 0; check[1] = 0; check[2] = 0; check[3] = 0;
		res.resample(vec);
		for (Particle<MyState>& p : vec) {
			++check[(int) p.state.y - 1];
		}

		//std::cout << check[0] << ":" <<check[1] << ":" << check[2] << ":" << check[3] << std::endl;

		// check particle distribution
		ASSERT_NEAR(0.1*cnt, check[0], 20);
		ASSERT_NEAR(0.3*cnt, check[1], 15);
		ASSERT_NEAR(0.6*cnt, check[2], 15);
		ASSERT_NEAR(0.0*cnt, check[3], 2);

	}




	class MyTransition : public ParticleFilterTransition<MyState> {
	public:
		NormalDistribution nd;
		MyTransition() : nd(0, 4) {;}
		void transition(std::vector<Particle<MyState>>& particles) {
			for (Particle<MyState>& p : particles) {
				p.state.x += nd.draw();
				p.state.y += nd.draw();
			}
		}
	};

	class MyEvaluation : public ParticleFilterEvaluation<MyState, MyObservation> {
		double evaluation(std::vector<Particle<MyState>>& particles, const MyObservation& o) {
			double sum = 0;
			for (Particle<MyState>& p : particles) {
				p.weight =	NormalDistribution::getProbability(o.x, 3, p.state.x) *
							NormalDistribution::getProbability(o.y, 3, p.state.y);
				sum += p.weight;
			}
			return sum;
		}
	};

	class MyInitializer : public ParticleFilterInitializer<MyState> {
		void initialize(std::vector<Particle<MyState>>& particles) override {
			for (Particle<MyState>& p : particles) {
				p.state.x = 0;
				p.state.y = 0;
			}
		}
	};

	TEST(Particles, filter) {

		ParticleFilter<MyState, MyObservation> pf(20000, std::unique_ptr<MyInitializer>(new MyInitializer));
		pf.setResampling(std::unique_ptr<ParticleFilterResamplingSimple<MyState>>(new ParticleFilterResamplingSimple<MyState>));
		pf.setEstimation(std::unique_ptr<ParticleFilterEstimationWeightedAverage<MyState>>(new ParticleFilterEstimationWeightedAverage<MyState>));

		//pf.setResampling(new ParticleResamplingNone<MyState>());
		//pf.setEstimation(new ParticleFilterEstimationMax<MyState>());

		pf.setTransition(std::unique_ptr<MyTransition>(new MyTransition));
		pf.setEvaluation(std::unique_ptr<MyEvaluation>(new MyEvaluation));

		MyObservation o;

		MyState s;

		// 1st run
		pf.init();
		o.set(50, 30);
		for (int i = 0; i < 8; ++i) {s = pf.update(o);}
		ASSERT_NEAR(50, s.x, 0.1);
		ASSERT_NEAR(30, s.y, 0.1);

//		// 2nd run
//		pf.init();
//		o.set(30, 50);
//		for (int i = 0; i < 8; ++i) {s = pf.update(o);}
//		ASSERT_NEAR(30, s.x, 0.1);
//		ASSERT_NEAR(50, s.y, 0.1);

//		// with max evaluation
//		pf.init();
//		o.set(20, 60);
//		pf.setEstimation(std::unique_ptr<ParticleFilterEstimationMax<MyState>>(new ParticleFilterEstimationMax<MyState>));
//		for (int i = 0; i < 8; ++i) {s = pf.update(o);}
//		ASSERT_NEAR(20, s.x, 0.1);
//		ASSERT_NEAR(60, s.y, 0.1);

//		// with regional weighted average evaluation
//		pf.init();
//		o.set(70, 10);
//		pf.setEstimation(std::unique_ptr<ParticleFilterEstimationRegionalWeightedAverage<MyState>>(new ParticleFilterEstimationRegionalWeightedAverage<MyState>));
//		for (int i = 0; i < 8; ++i) {s = pf.update(o);}
//		ASSERT_NEAR(70, s.x, 0.1);
//		ASSERT_NEAR(10, s.y, 0.1);

//		// with no resampling and max particle (VERY UNSTABLE!)
//		pf.init();
//		o.set(11, 22);
//		pf.setEstimation(std::unique_ptr<ParticleFilterEstimationMax<MyState>>(new ParticleFilterEstimationMax<MyState>));
//		pf.setResampling(std::unique_ptr<ParticleResamplingNone<MyState>>(new ParticleResamplingNone<MyState>));
//		for (int i = 0; i < 8; ++i) {s = pf.update(o);}
//		ASSERT_NEAR(11, s.x, 1);
//		ASSERT_NEAR(22, s.y, 1);


	}

}


#endif
