#ifndef K_MATH_OPT_NUMOPTALGOGENETIC_H_
#define K_MATH_OPT_NUMOPTALGOGENETIC_H_


#include "NumOptAlgo.h"
#include "NumOptVector.h"

#include <vector>

namespace K {

	template <int numArgs> class NumOptAlgoGenetic : public NumOptAlgo<numArgs> {

	private:

		/** the size of the population (number of elements) */
		int populationSize = 200;

		/** the maximum number of iterations */
		int maxIterations = 500;

		/** the random value range for mutation */
		float valRange = 15.0f;


	public:

		/** ctor */
		NumOptAlgoGenetic() {
			;
		}

		/** abort after the given number of iterations */
		void setMaxIterations(const unsigned int max) {
			this->maxIterations = max;
		}

		/** set the random-value-range used for mutation */
		void setValRange(const float range) {
			valRange = range;
		}

		/** set the population size to use */
		void setPopulationSize(const int num) {
			populationSize = num;
		}


		/**
		 * how it works:
		 * 1) start with X entites filled with Y random values
		 * 2) calculate their fitness (here: error)
		 * 3) sort by error (least error = best)
		 * 4) remove the worst elements and replace them by
		 * 5) a crossover between the two of the best element (50:50 chance to get field x from either A or B) with an additional chance of mutation!
		 * 6) goto 2 until x new creatures were born
		 */
		void calculateOptimum(NumOptFunction<numArgs>& func, NumOptVector<numArgs>& dst) override {

			// represents one entry within the population
			struct Entity {
				NumOptVector<numArgs> genes;
				float fitness;					// fitness (here: negative fitness: error)
			};

			// allocate space for the whole population
			std::vector<Entity> population;
			population.resize(populationSize);

			// initialize the poulation
			for (int p = 0; p < populationSize; ++p) {

				// start with the given initial genes (if any)
				population[p].genes = dst;

				// add random mutation to ALL genes
				mutate(population[p].genes);

				// calculate the fitness
				population[p].fitness = getFitness(func, population[p].genes);

			}


			for (int iter = 0; iter < maxIterations; ++iter) {

				float test = 0;
				for (int xx = 0; xx < populationSize; ++xx) {
					test += population[xx].fitness;
				}
				std::cout << "avg: " << (test / populationSize) << std::endl;

				const int numParents = populationSize * 0.25;
				const int numChildren = populationSize * 0.75;

				// replace the bad ones with new children by crossing good genes
				for (int cnt = 0; cnt < numChildren; ++cnt) {

					const int idx = populationSize - 1 - cnt;	// the child to create (the to-be-replaced bad-ones come last)
					const int p1 = randI(0, numParents);		// the childs 1st parent
					const int p2 = randI(0, numParents);		// the childs 2nd parent

//					if (spawn()) {

//						initRandom(population[idx].genes);

//					} else {

						// evoluation (and mutation)
						cross(population[p1].genes, population[p2].genes, population[idx].genes);

						// calculate the child's fitness
						population[idx].fitness = getFitness(func, population[idx].genes);

//					}


				}

				// sort by fitness (better ones (higher fitness) come first)
				auto lambda = [] (const Entity& e1, Entity& e2) {return e1.fitness > e2.fitness;};
				std::sort(std::begin(population), std::end(population), lambda);

			}

			// use the best chromosome as final result
			dst = population[0].genes;




		}



	private:

		/** mutate all of the given genes */
		inline void mutate(NumOptVector<numArgs>& genes) {
			for (int x = 0; x < numArgs; ++x) {
				genes[x] += randGene();
			}
		}

		/** calculate the fitness for the given genes */
		inline float getFitness(NumOptFunction<numArgs>& func, const NumOptVector<numArgs>& genes) {
			return - func.getValue(genes); // negative value to convert from "error" to "fitness"
		}

		/** cross the given two parent's genes to create an new child */
		void cross(const NumOptVector<numArgs>& parent1, const NumOptVector<numArgs>& parent2, NumOptVector<numArgs>& child) {

			// process each gene
			for (int i = 0; i < numArgs; ++i) {
				if (mutate()) {
					//  mutate this gene
					child[i] += randGene();
				} else {
					// take this gene from either parent1 or parent2
					child[i] = (randB()) ? (parent1[i]) : (parent2[i]);
				}
			}

		}

		/** get a random gene */
		inline float randGene() {
			return randF(-valRange, +valRange);
		}

		/** true in 2.0% of all runs */
		inline bool mutate() {
			return rand() < (RAND_MAX * 0.02);
		}

		/** true in 2.0% of all runs */
		inline bool spawn() {
			return rand() < (RAND_MAX * 0.02);
		}
				const float children = 0.9;
		/** random true/false (50:50) */
		inline bool randB() {
			return rand() < (RAND_MAX/2);
		}

		inline int randI(const int min, const int max) {
			return min + (uint64_t) rand() * (max-min) / RAND_MAX;
		}

		/** random value between min and max */
		inline float randF(const float min, const float max) {
			return min + rand() * (max-min) / RAND_MAX;
		}



	};

	}

#endif // K_MATH_OPT_NUMOPTALGOGENETIC_H_

