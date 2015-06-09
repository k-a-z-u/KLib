/*
 * TestParticles.cpp
 *
 *  Created on: Sep 18, 2013
 *      Author: Frank Ebner
 */


#ifdef FIXME

#ifdef WITH_TESTS

#include "Test.h"

#include "../particles/ParticleFilter.h"
#include "../particles/ParticleModel.h"
#include "../particles/ParticleSensor.h"
#include "../wifi/math/WiFiMath.h"
#include "../lib/gnuplot/Gnuplot.h"
#include <iostream>
#include "../floorplan/FloorPlan.h"
#include "../floorplan/FloorPlanFactory.h"
#include "../lib/misc/File.h"
#include "../particles/ParticleMath.h"
#include "../particles/resampling/ParticleResamplingSimple.h"
#include "../particles/resampling/ParticleResamplingNone.h"

#include "../wifi/factory/WiFiHelper.h"

#include <chrono>
#include <thread>

typedef Point TestState;


/** dump particles */
void dump(const ParticleFilter<TestState>& pf, FloorPlan& fp, Gnuplot& gp) {

	std::vector<Particle<TestState>> vec = pf.getParticles();

	gp << "plot '-' using 1:2:3 title '' ls 100 linecolor palette, \\\n";
	gp << "     '-' using 1:2   title '' ls 1   with lines, \\\n";
	gp << "     '-' using 1:2   title '' ls 50  \n";

	// particles
	const auto& lambda = [] (const Particle<TestState>& a, const Particle<TestState>& b) {return a.probability < b.probability;};
	double max = std::max_element(vec.begin(), vec.end(), lambda)->probability;
	for (const Particle<TestState>& p : vec) {
		double size = (p.probability/max);
		//if (size < 0.75) {size = 0.75;}
		gp << p.state.x << " " << p.state.y << " " << size << "\n";
	}
	gp << "e\n";

	// floorplan
	for (const Obstacle& o : fp.getObstacles()) {
		if (o.material == ObstacleMaterial::UNKNOWN) {continue;}
		if (o.material == ObstacleMaterial::DOOR) {continue;}
		gp << o.p1.x << " " << o.p1.y << "\n";
		gp << o.p2.x << " " << o.p2.y << "\n\n";
	}
	gp << "e\n";

	// mean
	TestState mean = pf.getMean();
	TestState weightedMean = pf.getWeightedMean();
	gp << mean.x << " " << mean.y << "\n";
	gp << weightedMean.x << " " << weightedMean.y << "\n";
	gp << "e\n";

	gp.flush();

}

TEST(Particles, init) {

	return;

	File file("/apps/master/messwerte/fh/fh3.svg");
	FloorPlan fp;
	FloorPlanFactory::loadFromSVG(file, fp);

	Gnuplot gp;

	gp << "set xrange [0:"<<fp.getWidth()<<"]\n";
	gp << "set yrange [0:"<<fp.getHeight()<<"]\n";
	gp << "unset colorbox\n";

	gp << "set style line 100 linecolor rgb '#000000' pointtype 7 pointsize 0.3\n";
	gp << "set style line 50 linecolor rgb '#0000FF' pointtype 3 \n";
	gp << "set style line 1 linecolor rgb '#666666'\n";

	//gp << "set palette defined (0 '#666666', 1 '#CCCCCC', 2 '#000000', 3 '#FF0000')\n";
	gp << "set palette defined (0 '#CCCCCC', 1 '#000000')\n";

	struct Model : public ParticleModel<TestState> {
		const FloorPlan fp;
		Model(const FloorPlan& fp) : fp(fp) {;}
		void modify(TestState& s) const override {

			again:;
			float speed = 1.5 * 100;	// centimeters/second
			float elapsed = 1.0;//(getTimeStampMS() - s.lastTs) / 1000.0;
			//s.lastTs = getTimeStampMS();
			speed *= elapsed;

			Point p1 = s;				// current location
			Point p2;					// new location

			//double rnd = ParticleMath::getRandom(0,1);
			//if (rnd > 0.95) {speed *= 5;}

			double rndSpeed = ParticleMath::getNormalDistributionRandom(speed, speed/2);
			double rndAngle = ParticleMath::getRandom(0, 2*3.1415);
			float dx = (float) (cos(rndAngle) * rndSpeed);
			float dy = (float) (sin(rndAngle) * rndSpeed);
			p2.set(s.x+dx,s.y+dy);


			// keep particles within map
			if (p2.x < 0 || p2.x > fp.getWidth()) {goto again;}
			if (p2.y < 0 || p2.y > fp.getHeight()) {goto again;}

			// perform collision detection
			std::vector<Obstacle> obstacles = fp.getObstacles(Line(p1, p2));
			for (const Obstacle& o : obstacles) {
				if (o.material != ObstacleMaterial::DOOR && o.material != ObstacleMaterial::UNKNOWN) {goto again;}
			}


			// done
			s.x = p2.x;
			s.y = p2.y;

		}
		void init(TestState& s) const {

			// evenly distribute within map
			//s.x = ParticleMath::getRandom(0, fp.getWidth());
			//s.y = ParticleMath::getRandom(0, fp.getHeight());

			// normal-distribute particles (start in the lower left)
			s.x = ParticleMath::getNormalDistributionRandom(500, 50);
			s.y = ParticleMath::getNormalDistributionRandom(500, 50);

		}
	};

	struct : public ParticleSensor<TestState> {
		double getProbability(const TestState& s) const override {
			double d1 = WiFiMath::getNormalDistribution1D(4000, 1000, s.x);
			double d2 = WiFiMath::getNormalDistribution1D(5000, 1000, s.y);
			return d1*d2;
		}
	} sensor1;

	Model model(fp);

	ParticleResamplingSimple<TestState> resampler;
	ParticleFilter<TestState> pf(model, resampler);
	pf.init(500);
	pf.addSensor(&sensor1);

	while(true) {

		dump(pf, fp, gp);

		//		char tmp;
		//		std::cin >> tmp;
		//usleep(100*1000);
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

		pf.update();

	}

	exit(0);

}

#endif

#endif
