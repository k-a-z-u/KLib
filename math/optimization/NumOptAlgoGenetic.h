#ifndef K_MATH_OPT_NUMOPTALGOGENETIC_H_
#define K_MATH_OPT_NUMOPTALGOGENETIC_H_


#include "NumOptAlgo.h"
//#include "NumOptVector.h"

#include "../statistics/Statistics.h"

#include <vector>
#include <random>
#include <functional>

#include "../distribution/Normal.h"

namespace K {


	template <typename Scalar> class NumOptAlgoGenetic {

	public:

		struct MinMax {
			Scalar min;
			Scalar max;
			MinMax(const Scalar min, const Scalar max) : min(min), max(max) {;}
			const Scalar diff() const {return max - min;}
		};

	private:

		/** number of parameters */
		int numParams;

		/** the size of the population (number of elements) */
		int populationSize = 200;

		/** the maximum number of iterations */
		int maxIterations = 500;

		/** the best x% of each population survive without modification */
		float elitism = 0.02f;

		/** chance of mutating a gene during crossover */
		float mutation = 0.10f;

		/** the random value range for mutation */
		std::vector<Scalar> valRange;

		/** the region [min:max] values are allowed within */
		std::vector<MinMax> valRegion;

		/** random number generator */
		std::minstd_rand gen;

		/** an exponential distribution for picking random parents */
		std::exponential_distribution<float> expDist = std::exponential_distribution<float>(10.0);

		/** callback-function to inform after every run */
		std::function<void(const int iteration, const Scalar error, const Scalar* values)> callback;

	public:

		/** ctor */
		NumOptAlgoGenetic(const int numParams) : numParams(numParams) {
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
		void setValRange(const Scalar range) {
			valRange.resize(numParams);
			for (int i = 0; i < numParams; ++i) {valRange[i] = range;}
		}

		/** set the random-value-range used for mutation */
		void setValRange(const std::vector<Scalar>& range) {
			this->valRange = range;
		}

		/** limit the region values are allowed within, and set the random range for initial values */
		void setValRegion(const std::vector<MinMax>& region) {
			this->valRegion = region;
			this->valRange.clear();
			for (const MinMax& mm : region) {
				const float range = mm.diff() / 33.0;
				this->valRange.push_back(range);			// if region is e.g. [-3:+4] use 7 / 33 as random modification
			}
		}

		/** represents one entry within the population */
		struct Entity {
			Scalar* genes;					// pointer to an area providing enough space for the genes
			Scalar fitness;					// fitness (here: negative fitness: error)
		};

		/** set a callback-function to inform after every run */
		void setCallback(std::function<void(const int iteration, const Scalar error, const Scalar* params)> func) {
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
		template <typename Func> void calculateOptimum(Func& func, Scalar* dst) {

			// allocate space for the whole population and its genes (twice: current and next)
			std::vector<Entity> currentPopulation;			currentPopulation.resize(populationSize);
			std::vector<Scalar> currentPopulationGenes;		currentPopulationGenes.resize(populationSize * numParams);
			for (int i = 0; i < populationSize; ++i)		{currentPopulation[i].genes = &currentPopulationGenes[i*numParams];}

			std::vector<Entity> nextPopulation;				nextPopulation.resize(populationSize);
			std::vector<Scalar> nextPopulationGenes;		nextPopulationGenes.resize(populationSize * numParams);
			for (int i = 0; i < populationSize; ++i)		{nextPopulation[i].genes = &nextPopulationGenes[i*numParams];}

			// number of children to survive within each generation
			const int toSurvive = (int) std::ceil(elitism * populationSize);	// ceil: round up to at-least 1

			// initialize the 1st poulation
			// one entry is the given parameter-set (Scalar* dst)
			// all other entries are random within the provided valueRegion (setValRegion())
			for (int p = 0; p < populationSize; ++p) {

				// first entry = parameters as-is
				if (p == 0) {

					// start with the given initial genes (if any)
					std::copy(&dst[0], &dst[numParams], currentPopulation[p].genes);

				// all other entries = truely random
				} else {

					// initialize with random values, but use the given values (if any) as a hint
					initWithRandomValues(currentPopulation[p], dst);

				}

				// calculate the fitness for the chosen random values
				currentPopulation[p].fitness = getFitness(func, currentPopulation[p].genes);

			}

			// sort by fitness
			sort(currentPopulation);

			// generate X new popultions
			for (int iter = 0; iter < maxIterations; ++iter) {

				int childIdx = 0;

				// if configured: let the best x percent survive without recombination / mutation
				for (; childIdx < toSurvive; ++childIdx) {
					//nextPopulation[childIdx] = currentPopulation[childIdx];			// copy into the new population
					nextPopulation[childIdx].fitness = currentPopulation[childIdx].fitness;
					std::copy(&currentPopulation[childIdx].genes[0], &currentPopulation[childIdx].genes[numParams], nextPopulation[childIdx].genes);
				}

				// create the new population by crossing and mutating the current one

				//while (childIdx < populationSize) {
				#pragma omp parallel for
				for ( int _childIdx = childIdx; _childIdx < populationSize; ++_childIdx) {

					// find two parents
					const int p1 = randomParent();		// the new childs 1st parent
					const int p2 = randomParent();		// the new childs 2nd parent

					// evoluation: crossing and mutation
					crossAndMutate(currentPopulation[p1].genes, currentPopulation[p2].genes, nextPopulation[_childIdx].genes);

					// calculate the new child's fitness
					nextPopulation[_childIdx].fitness = getFitness(func, nextPopulation[_childIdx].genes);

					// next
					//++childIdx;

				}

				// swap current and next generation
				currentPopulation.swap(nextPopulation);

				// sort by fitness
				sort(currentPopulation);

				// inform callback (if any) about the current optimum
				if (callback) {
					callback(iter, currentPopulation[0].fitness, currentPopulation[0].genes);
				}

			}

			// use the best chromosome as final result
			std::copy(&currentPopulation[0].genes[0], &currentPopulation[0].genes[numParams], dst);

		}



	private:

		/**
		 * init the given genes with random values
		 * based on the given hint or provided valRegion values (if any)
		 */
		void initWithRandomValues(Entity& e, Scalar* hint) {

			// valRegion setting available? use it!
			if (!this->valRegion.empty()) {

				for (int i = 0; i < numParams; ++i) {
					std::uniform_real_distribution<Scalar> dist(valRegion[i].min, valRegion[i].max);
					const Scalar val = dist(gen);
					e.genes[i] = val;
				}

			// no valRegion provided. use a mutated hint as fallback
			} else {

				std::copy(&hint[0], &hint[numParams], e.genes);
				mutate(e.genes);

			}

		}

		/** sort population by fitness (better ones [higher fitness] come first) */
		inline void sort(std::vector<Entity>& population) {
			static auto lambda = [] (const Entity& e1, const Entity& e2) {return e1.fitness > e2.fitness;};
			std::sort(population.begin(), population.end(), lambda);
		}

		/** get a random parent index using an exponential distribution (favoring lower (=fitter) indicies) */
		inline int randomParent() {
			const float r01 = expDist(gen);
			const int idx = r01 * populationSize;
			return (idx >= populationSize) ? (populationSize-1) : (idx);
		}

		/** mutate all of the given genes */
		template <typename Params> inline void mutate(Params& genes) {
			for (int x = 0; x < numParams; ++x) {
				genes[x] += randGene(x);
			}
		}

		/** calculate the fitness for the given genes */
		template <typename Func, typename Params> static inline float getFitness(Func& func, const Params& genes) {
			return - (func(genes)); // negative value to convert from "error" to "fitness"
		}

		/** cross the given two parent's genes and apply random mutation to create an new child */
		template <typename Params> inline void crossAndMutate(const Params& parent1, const Params& parent2, Params& child) {

			// process each gene
			for (int i = 0; i < numParams; ++i) {

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
			std::normal_distribution<Scalar> d(0, sigma);
			return d(gen);
		}

	};

	}

#endif // K_MATH_OPT_NUMOPTALGOGENETIC_H_

