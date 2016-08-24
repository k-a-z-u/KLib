#ifndef K_MATH_FILTER_PARTICLES_PARTICLEFILTERESTIMATIONKERNELDENSITY_H
#define K_MATH_FILTER_PARTICLES_PARTICLEFILTERESTIMATIONKERNELDENSITY_H


#include "ParticleFilterEstimation.h"
#include <algorithm>
#include <vector>
#include "../Particle.h"

#include "../../../../misc/gnuplot/Gnuplot.h"
#include "../../../optimization/NumOptVector.h"
#include "../../../optimization/NumOptAlgoDownhillSimplex.h"



namespace K {

	template <typename State, int numParams>
	class ParticleFilterEstimationKernelDensity : public ParticleFilterEstimation<State> {

		Gnuplot gp;

		K::NumOptAlgoDownhillSimplex<float, numParams> simplex;

	private:

		class OptFunc  {

		private:

			/** the particles to work on */
			const std::vector<Particle<State>>& particles;

		public:

			/** ctor */
			OptFunc(const std::vector<Particle<State>>& particles) : particles(particles) {;}

			float operator () (const float* params) const {

				double prob = 0;
				const int size = particles.size();

				//#pragma omp parallel for
				for (int i = 0; i < size; i+=10) {
					const Particle<State>& p = particles[i];
					prob += p.state.getKernelDensityProbability(params) * p.weight;
				}

				// convert probability to "error"
				return -prob;

			}

		};


	public:

		State estimate(std::vector<Particle<State>>& particles) override {


			// comparator
			auto comp = [] (const Particle<State>& p1, const Particle<State>& p2) { return p1.weight < p2.weight; };

//			// find max state
//			auto el = std::max_element(particles.begin(), particles.end(), comp);
//			State max =  el->state;


//			// region to check
//			BBox2 bbox;
//			bbox.add(Point2f(-50,-50));
//			bbox.add(Point2f(100,100));

//			const float stepSize = 1.0f;

//			const int pxX = (bbox.getMax().x - bbox.getMin().x) / stepSize;
//			const int pxY = (bbox.getMax().y - bbox.getMin().y) / stepSize;

//			// optimize using simplex
			OptFunc func(particles);

//			// calculate the optimum
//			simplex.calculateOptimum(func, params);
//			std::cout << params << std::endl;


			float params[numParams];
			getGlobalMax(particles, params);

			// create output state from optimized params
			State res(params);


			gp << "splot '-' with lines\n";

			int x1 = 0;//params[0]-2500;
			int x2 = 100*100;//params[0]+2500;
			int y1 = 0;//params[1]-2500;
			int y2 = 60*100;//params[1]+2500;

			for (int x = x1; x < x2; x += 400) {
				for (int y = y1; y < y2; y += 400) {
					params[0] = x;
					params[1] = y;
					params[2] = 0;
					gp << x << " " << y << " " << -func(params) << "\n";
				}
				gp << "\n";
			}
			gp << "e\n";
			gp.flush();

			return res;

		}

		void getGlobalMax(const std::vector<Particle<State>>& particles, float* startParams) {

			OptFunc func(particles);
			double bestP = 0;
			float bestParams[numParams];

			simplex.setMaxIterations(10);
			simplex.setNumRestarts(0);


			for (int i = 0; i < 15; ++i) {

				// start at a random particle
				const int idx = rand() % particles.size();

				// start optimization at this particle's paramters
				particles[idx].state.fillKernelDenstityParameters(startParams);

				simplex.calculateOptimum(func, startParams);

				const float prob = -func(startParams);
				if (prob > bestP) {
					bestP = prob;
					memcpy(bestParams, startParams, numParams*sizeof(float));
					//std::cout << bestParams << std::endl;
				}

			}

			memcpy(startParams, bestParams, numParams*sizeof(float));

		}

	};

}

#endif // K_MATH_FILTER_PARTICLES_PARTICLEFILTERESTIMATIONKERNELDENSITY_H
