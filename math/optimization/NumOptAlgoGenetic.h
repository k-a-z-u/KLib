#ifndef K_MATH_OPT_NUMOPTALGOGENETIC_H_
#define K_MATH_OPT_NUMOPTALGOGENETIC_H_


#include "NumOptAlgo.h"
#include "NumOptVector.h"

#include "../statistics/Statistics.h"

#include <vector>
#include <random>
#include <functional>

#include "../distribution/Normal.h"

namespace K {

	template <int numArgs> class NumOptAlgoGenetic : public NumOptAlgo<numArgs> {

	private:

		/** the size of the population (number of elements) */
		int populationSize = 200;

		/** the maximum number of iterations */
		int maxIterations = 500;

		/** the best x% of each population survive without modification */
		float elitism = 0.02f;

		/** chance of mutating a gene during crossover */
		float mutation = 0.10f;

		/** the random value range for mutation */
		NumOptVector<numArgs> valRange;

		/** random number generator */
		std::minstd_rand gen;

		/** an exponential distribution for picking random parents */
		std::exponential_distribution<float> expDist = std::exponential_distribution<float>(10.0);

		/** callback-function to inform after every run */
		std::function<void(const int iteration, const float fitness, const NumOptVector<numArgs>&)> callback;

	public:

		/** ctor */
		NumOptAlgoGenetic() {
			_assertEqual(1, gen.min(), "random number generator's minimum is invalid");
			gen.seed(1234);
			setValRange(1.0);
		}

		/** abort after the given number of iterations */
		void setMaxIterations(const unsigned int max) {
			this->maxIterations = max;
		}

		/** set the population size to use */
		void setPopulationSize(const int num) {
			populationSize = num;
		}

		/** configure whether to use elitism: (unmodified!) survival of the fittest. [0.0:1.0] */
		void setElitism(const float elitism) {
			this->elitism = elitism;
		}

		/** set the chance of mutation [0.0:1.0] */
		void setMutation(const float mutation) {
			this->mutation = mutation;
		}

		/** set the random-value-range used for mutation */
		void setValRange(const float range) {
			for (int i = 0; i < numArgs; ++i) {valRange[i] = range;}
		}

		/** set the random-value-range used for mutation */
		void setValRange(const NumOptVector<numArgs>& range) {
			this->valRange = range;
		}


		/** represents one entry within the population */
		struct Entity {
			NumOptVector<numArgs> genes;
			float fitness;					// fitness (here: negative fitness: error)
		};

		/** set a callback-function to inform after every run */
		void setCallback(std::function<void(const int iteration, const float fitness, const NumOptVector<numArgs>&)> func) {
			this->callback = func;
		}

		/**
		 * how it works:
		 * 1) start with X entites filled with Y random values
		 * 2) calculate their fitness (he>re: error)
		 * 3) sort by error (least error = best)
		 * 4) create a new population by
		 *		a crossover between two current parents (genes), depending on their fitness
		 *		and a slight chance of mutation
		 * 6) repeat until X populations were created
		 */
		void calculateOptimum(NumOptFunction<numArgs>& func, NumOptVector<numArgs>& dst) override {

			// allocate space for the whole population (twice: current and next)
			std::vector<Entity> currentPopulation;		currentPopulation.resize(populationSize);
			std::vector<Entity> nextPopulation;			nextPopulation.resize(populationSize);

			// number of children to survive within each generation
			const int toSurvive = (int) std::ceil(elitism * populationSize);	// ceil: round up to at-least 1

			// initialize the 1st poulation
			for (int p = 0; p < populationSize; ++p) {

				// start with the given initial genes (if any)
				currentPopulation[p].genes = dst;

				// the first entry of the population remains as-is (elitism) the others are mutated
				if (p > 0) {

					// add random mutation to all genes
					mutate(currentPopulation[p].genes);

				}

				// calculate the fitness
				currentPopulation[p].fitness = getFitness(func, currentPopulation[p].genes);

			}

			// sort by fitness
			sort(currentPopulation);

			// generate X new popultions
			for (int iter = 0; iter < maxIterations; ++iter) {

				int childIdx = 0;

				// if configured: let the best x percent survive without recombination / mutation
				for (; childIdx < toSurvive; ++childIdx) {
					nextPopulation[childIdx] = currentPopulation[childIdx];			// copy into the new population
				}

				// create the new population by crossing and mutating the current one
				while (childIdx < populationSize) {

					// find two parents
					const int p1 = randomParent();		// the new childs 1st parent
					const int p2 = randomParent();		// the new childs 2nd parent

					// evoluation: crossing and mutation
					crossAndMutate(currentPopulation[p1].genes, currentPopulation[p2].genes, nextPopulation[childIdx].genes);

					// calculate the new child's fitness
					nextPopulation[childIdx].fitness = getFitness(func, nextPopulation[childIdx].genes);

					// next
					++childIdx;

				}

				// swap current and next generation
				currentPopulation.swap(nextPopulation);

				// sort by fitness
				sort(currentPopulation);


				K::Statistics<float> stats;
				for (int xx = 0; xx < populationSize; ++xx) {
					//test += currentPopulation[xx].fitness;
					stats.add(currentPopulation[xx].fitness);
				}
				//(std::cout) << std::endl;
				//std::cout << currentPopulation[0].genes << std::endl;
				//std::cout << "avg: " << (test / populationSize) << std::endl;

				// inform callback (if any) about the current optimum
				if (callback) {
					callback(iter, currentPopulation[0].fitness, currentPopulation[0].genes);
				}

			}

			// use the best chromosome as final result
			dst = currentPopulation[0].genes;
			std::cout << dst << std::endl;
			std::cout << currentPopulation[0].fitness << std::endl;
			std::cout << getFitness(func, dst) << std::endl;
			std::cout << getFitness(func, currentPopulation[0].genes) << std::endl;
			std::cout << getFitness(func, currentPopulation[0].genes) << std::endl;
			std::cout << getFitness(func, currentPopulation[0].genes) << std::endl;
			std::cout << getFitness(func, currentPopulation[0].genes) << std::endl;
			std::cout << getFitness(func, currentPopulation[0].genes) << std::endl;

		}



	private:

		/** sort population by fitness (better ones [higher fitness] come first) */
		inline void sort(std::vector<Entity>& population) {
			static auto lambda = [] (const Entity& e1, const Entity& e2) {return e1.fitness > e2.fitness;};
			std::sort(std::begin(population), std::end(population), lambda);
		}

		/** get a random parent index using an exponential distribution favoring lower indicies */
		inline int randomParent() {
			const float r01 = expDist(gen);
			const int idx = r01 * populationSize;
			return (idx >= populationSize) ? (populationSize-1) : (idx);
		}

		/** mutate all of the given genes */
		inline void mutate(NumOptVector<numArgs>& genes) {
			for (int x = 0; x < numArgs; ++x) {
				genes[x] += randGene(x);
			}
		}

		/** calculate the fitness for the given genes */
		inline float getFitness(NumOptFunction<numArgs>& func, const NumOptVector<numArgs>& genes) {
			return - func.getValue(genes); // negative value to convert from "error" to "fitness"
		}

		/** cross the given two parent's genes and apply random mutation to create an new child */
		inline void crossAndMutate(const NumOptVector<numArgs>& parent1, const NumOptVector<numArgs>& parent2, NumOptVector<numArgs>& child) {

			// process each gene
			for (int i = 0; i < numArgs; ++i) {

				// take this gene from either parent1 or parent2
				child[i] = (rand5050()) ? (parent1[i]) : (parent2[i]);

				//  mutate this gene?
				if (doMutate()) {child[i] += randGene(i);}

			}

		}

		/** get a random genetic change depending on the index and thus allowed variance for this gene */
		inline float randGene(const int idx) {
			return randF(valRange[idx]);
		}

		/** true in x% of all calls */
		inline bool doMutate() {
			return gen() < (gen.max() * mutation);
		}

		/** random true/false (50:50) */
		inline bool rand5050() {
			return gen() < (gen.max() / 2);
		}


		/** zero-mean normally-distributed random value */
		inline float randF(const float sigma) {
			std::normal_distribution<> d(0, sigma);
			return d(gen);
		}

	};

	}

#endif // K_MATH_OPT_NUMOPTALGOGENETIC_H_

