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

		K::NumOptAlgoDownhillSimplex<numParams> simplex;

	private:

		class OptFunc : public NumOptFunction<numParams> {

		private:

			/** the particles to work on */
			const std::vector<Particle<State>>& particles;

		public:

			/** ctor */
			OptFunc(const std::vector<Particle<State>>& particles) : particles(particles) {;}

			virtual double getValue(const NumOptVector<numParams>& args) const {

				double prob = 0;
				const int size = particles.size();

				#pragma omp parallel for
				for (int i = 0; i < size; ++i) {
					const Particle<State>& p = particles[i];
					prob += p.state.getKernelDensityProbability(args) * p.weight;
				}

				return -prob;

			}

		};


	public:

		State estimate(std::vector<Particle<State>>& particles) override {


			// comparator
			auto comp = [] (const Particle<State>& p1, const Particle<State>& p2) { return p1.weight < p2.weight; };

			// find max state
			auto el = std::max_element(particles.begin(), particles.end(), comp);
			State max =  el->state;

//			// start using the particle with the max-weight
//			NumOptVector<numParams> params;
//			max.fillKernelDenstityParameters(params);

//			// optimize using simplex
			OptFunc func(particles);



//			// calculate the optimum
//			simplex.calculateOptimum(func, params);
//			std::cout << params << std::endl;


			NumOptVector<numParams> params = getGlobalMax(particles);

			// create output state from optimized params
			State res(params);




			gp << "splot '-' with lines\n";

			int x1 = params[0]-1200;
			int x2 = params[0]+1200;
			int y1 = params[1]-1200;
			int y2 = params[1]+1200;

			for (int x = x1; x < x2; x += 300) {
				for (int y = y1; y < y2; y += 300) {
					params[0] = x;
					params[1] = y;
					params[2] = 3;
					gp << x << " " << y << " " << -func.getValue(params) << "\n";
				}
				gp << "\n";
			}
			gp << "e\n";
			gp.flush();




			return res;

		}

		NumOptVector<numParams> getGlobalMax(const std::vector<Particle<State>>& particles) {

			NumOptVector<numParams> startParams;
			OptFunc func(particles);
			double bestP = 0;
			NumOptVector<numParams> bestParams;

			simplex.setMaxIterations(15);
			simplex.setNumRestarts(1);


			for (int i = 0; i < 15; ++i) {

				int idx = rand() % particles.size();

				particles[idx].state.fillKernelDenstityParameters(startParams);

				simplex.calculateOptimum(func, startParams);

				double p = -func.getValue(startParams);
				if (p > bestP) {
					bestP = p;
					bestParams = startParams;
					std::cout << bestParams << std::endl;
				}

			}

			return bestParams;

		}

	};

}

#endif // K_MATH_FILTER_PARTICLES_PARTICLEFILTERESTIMATIONKERNELDENSITY_H
