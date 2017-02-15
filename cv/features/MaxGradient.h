#ifndef MAXGRADIENT_H
#define MAXGRADIENT_H

#include "../../geo/Size2.h"
#include "../../geo/Point2.h"
#include "../ImageChannel.h"
#include "../Derivative.h"


namespace K {

	class MaxGradient {

	public:

		struct Direction {
			float rad;	// direction in radians
			float mag;	// direction's strength
			Direction() : rad(0), mag(0) {;}
			Direction(const float rad, const float mag) : rad(rad), mag(mag) {;}
		};

		using Pattern = std::vector<float>;

	private:

		/** size of one cell to calculate the maximum gradient for */
		Size2i cellSize;

		/** size of the block [containing multiple cells] to calculate the feature from */
		Size2i blockSize;

		/** the stride to use for combining cells */
		Point2i cellStride;

		/** every precalculated cell */
		DataMatrix<Direction> cells;

		/** every precalculated block */
		DataMatrix<Pattern> blocks;

	public:

		/** ctor */
		MaxGradient(const K::ImageChannel& img, const Size2i cellSize = Size2i(9,9), const Size2i blockSize = Size2i(33,33), const Point2i cellStride = Point2i(3,3)) :
			cellSize(cellSize), blockSize(blockSize), cellStride(cellStride) {

			build(img);

		}

		const Direction& getCell(const int x, const int y) const {
			return cells.getConstRef(x,y);
		}

		const Pattern& getBlock(const int x, const int y) const {
			return blocks.getConstRef(x,y);
		}

		const Pattern& getPattern(const int x, const int y) const {
			return blocks.getConstRef(x,y);
		}

	private:

		/** pre-calculate everything */
		void build(const K::ImageChannel& img) {

			cells = buildCells(img);
			blocks = buildBlocks(cells);

		}

		inline int halfL(const int inp) const {
			return inp/2;
		}
		inline int halfH(const int inp) const {
			return (inp+1)/2;
		}

		/** build every n*m cell by calculating its biggest gradient */
		DataMatrix<Direction> buildCells(const ImageChannel& img) const {

			// output
			DataMatrix<Direction> cells(img.getWidth(), img.getHeight());

			// x/y derivative [-1, 0, +1]
			const K::ImageChannel imgX = K::Derivative::getXcen(img);
			const K::ImageChannel imgY = K::Derivative::getYcen(img);

			// process every pixel within the image
			for (int y = 0; y < img.getHeight(); ++y) {

				#pragma omp parallel for
				for (int x = 0; x < img.getWidth(); ++x) {

					// track the maximum gradient within the window
					float maxMag = 0;
					float maxDir = 0;

					// region
					const int wx1 = x -halfL(cellSize.w);
					const int wx2 = x +halfH(cellSize.w);
					const int wy1 = y -halfL(cellSize.h);
					const int wy2 = y +halfH(cellSize.h);

					// find the highest gradient within the window
					for (int y1 = wy1; y1 < wy2; ++y1) {
						for (int x1 = wx1; x1 < wx2; ++x1) {

							// clamp
							if (x1 < 0 || x1 >= img.getWidth())		{continue;}
							if (y1 < 0 || y1 >= img.getHeight())	{continue;}

							// calculate the centered derivatives
							const auto dx = imgX.get(x1, y1);	// gradient's magnitude in x direction
							const auto dy = imgY.get(x1, y1);	// gradient's magnitude in y direction

							const float mag = std::sqrt( (dx*dx) + (dy*dy) );		// gradient's overall magnitude
							const float dir = atan360(dy, dx);

							// keep the best
							if (mag > maxMag) {
								maxMag = mag;
								maxDir = dir;
							}

						}
					}


					const float dirRad = std::fmod(maxDir, M_PI);		// TODO: check

					// persist
					cells.set(x,y, Direction(dirRad, maxMag));

				}
			}

			// done
			return cells;

		}

		inline float atan360(const float dy, const float dx) const {
			const float rad = std::atan2(dy, dx);
			return (rad >= 0.0) ? (rad) : (2.0f*(float)M_PI+rad);
		}

		inline int clamp(const int val, const int min, const int max) const {
			if (val < min) {return min;}
			if (val > max) {return max;}
			return val;
		}

		DataMatrix<Pattern> buildBlocks(const DataMatrix<Direction>& cells) const {

			// output
			DataMatrix<Pattern> blocks(cells.getWidth(), cells.getHeight());

			// combine several descriptors
			for (int y = 0; y < cells.getHeight(); ++y) {

				#pragma omp parallel for
				for (int x = 0; x < cells.getWidth(); ++x) {

					Pattern& patOut = blocks.get(x,y);

					// region
					const int wx1 = x -halfL(blockSize.w);
					const int wx2 = x +halfH(blockSize.w) - cellSize.w;
					const int wy1 = y -halfL(blockSize.h);
					const int wy2 = y +halfH(blockSize.h) - cellSize.h;

					std::vector<Direction> dirs;

					for (int y1 = wy1; y1 <= wy2; y1 += cellStride.y) {
						for (int x1 = wx1; x1 <= wx2; x1 += cellStride.x) {

							const Direction in = cells.get(
								clamp(x1, 0, cells.getWidth()-1),
								clamp(y1, 0, cells.getHeight()-1)
							);

							dirs.push_back(in);

							//patOut.push_back(in.rad);
							//patOut.push_back(in.mag);

						}
					}

					auto lambda = [] (const Direction& d1, const Direction& d2) {return d1.mag < d2.mag;};
					std::sort(dirs.begin(), dirs.end(), lambda);
					for (const Direction dir : dirs) {
						patOut.push_back(dir.rad);
						patOut.push_back(dir.mag);
					}

				}
			}

			// done
			return blocks;

		}


	};

}

#endif // MAXGRADIENT_H
