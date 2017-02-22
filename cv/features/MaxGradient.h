#ifndef MAXGRADIENT_H
#define MAXGRADIENT_H

#include "../../geo/Size2.h"
#include "../../geo/Point2.h"
#include "../ImageChannel.h"
#include "../Derivative.h"
#include "Moments.h"

namespace K {

	class MaxGradient {

	public:

		struct Params {
			float rad;		// direction in radians
			float mag;		// direction's strength
			float avg;		// regions brightness average
			float stdDev;	// regions brightness std-dev
			float m00;
			float m11;
			Params() : rad(0), mag(0), avg(0), stdDev(0), m00(0), m11(0) {;}
			Params(const float rad, const float mag, const float avg, const float stdDev) : rad(rad), mag(mag), avg(avg), stdDev(stdDev) {;}
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
		DataMatrix<Params> cells;

		/** every precalculated block */
		DataMatrix<Pattern> blocks;

	public:

		/** ctor */
		MaxGradient(const K::ImageChannel& img, const Size2i cellSize = Size2i(9,9), const Size2i blockSize = Size2i(33,33), const Point2i cellStride = Point2i(3,3)) :
			cellSize(cellSize), blockSize(blockSize), cellStride(cellStride) {

			build(img);

		}

		const Params& getCell(const int x, const int y) const {
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

		static inline int halfL(const int inp) { return inp/2; }
		static inline int halfH(const int inp) { return (inp+1)/2; }

		/** build every n*m cell by calculating its biggest gradient */
		DataMatrix<Params> buildCells(const ImageChannel& img) const {

			// output
			DataMatrix<Params> cells(img.getWidth(), img.getHeight());

			// x/y derivative [-1, 0, +1]
			const K::ImageChannel imgX = K::Derivative::getXcen(img);
			const K::ImageChannel imgY = K::Derivative::getYcen(img);

			// process every pixel within the image
			for (int y = 0; y < img.getHeight(); ++y) {

				#pragma omp parallel for
				for (int x = 0; x < img.getWidth(); ++x) {

					Params p;

					// track the maximum gradient within the window
					float maxMag = 0;
					float maxDir = 0;
					float pxSum = 0;
					float pxSum2 = 0;
					int pxCnt = 0;

//					K::Point2f avgSum;
//					int avgCnt = 0;

					// region
					const int wx1 = -halfL(cellSize.w);
					const int wx2 = +halfH(cellSize.w);
					const int wy1 = -halfL(cellSize.h);
					const int wy2 = +halfH(cellSize.h);

					// find the highest gradient within the window
					for (int oy = wy1; oy < wy2; ++oy) {
						for (int ox = wx1; ox < wx2; ++ox) {

							//const float dFromCenter = std::sqrt(ox*ox + oy*oy);
							const float sigma = 7;
							const float mul = std::exp( - (float)((ox*ox)+(oy*oy)) / (2.0f*sigma*sigma) );
							const int x1 = x+ox;
							const int y1 = y+oy;

							// clamp
							if (x1 < 0 || x1 >= img.getWidth())		{continue;}
							if (y1 < 0 || y1 >= img.getHeight())	{continue;}

							// calculate the centered derivatives
							const auto dx = mul * imgX.get(x1, y1);	// gradient's magnitude in x direction
							const auto dy = mul * imgY.get(x1, y1);	// gradient's magnitude in y direction

							const float mag = std::sqrt( (dx*dx) + (dy*dy) );		// gradient's overall magnitude
							const float dir = atan360(dy, dx);

							// keep the best
							if (mag > maxMag) {
								maxMag = mag;
								maxDir = dir;
							}

//							avgSum += K::Point2f(dx, dy);
//							++avgCnt;

							const auto val = mul * img.get(x1, y1);	// raw image value
							pxSum += val;
							pxSum2 += val*val;
							++pxCnt;

						}
					}

					p.m00 = Moments::get<0,0>(img, Point2i(x,y), cellSize);
					p.m11 = Moments::get<1,1>(img, Point2i(x,y), cellSize);


					const float pxAvg = pxSum/pxCnt;
					const float pxStdDev = std::sqrt( (pxSum2/pxCnt) - (pxSum/pxCnt)*(pxSum/pxCnt) );

					// 180 degree regions
					const float dirRad = std::fmod(maxDir, M_PI);		// TODO: check

//					const K::Point2f avg = avgSum / avgCnt;
//					const float avgDir = std::fmod(atan360(avg.y, avg.x), M_PI);
//					const float mag = avg.getLength() * 3.5;

					// persist
					p.rad = dirRad;
					p.mag = maxMag;
					p.avg = pxAvg;
					p.stdDev = pxStdDev;
					cells.set(x,y, p);
					//cells.set(x,y, Params(avgDir, mag, pxAvg, pxStdDev));

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

		DataMatrix<Pattern> buildBlocks(const DataMatrix<Params>& cells) const {

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

					std::vector<Params> params;

					for (int y1 = wy1; y1 <= wy2; y1 += cellStride.y) {
						for (int x1 = wx1; x1 <= wx2; x1 += cellStride.x) {

							const Params in = cells.get(
								clamp(x1, 0, cells.getWidth()-1),
								clamp(y1, 0, cells.getHeight()-1)
							);

							params.push_back(in);

						}
					}

					//normalize(params);

					for (const Params& p : params) {

						patOut.push_back(p.rad / M_PI);
						patOut.push_back(p.mag);
						patOut.push_back(p.avg);
						patOut.push_back(std::sqrt(p.stdDev));

						//const int div = cellSize.w * cellSize.h;
						//patOut.push_back(p.m00 / div);
						//patOut.push_back(std::sqrt(p.m11) / div);
					}


				}
			}

			// done
			return blocks;

		}

		static void normalize(std::vector<Params>& vec) {

			float sumMag = 0;
			float sumStdDev = 0;

			for (const Params& p : vec) {
				sumMag += p.mag * p.mag;
				sumStdDev += p.stdDev * p.stdDev;
			}

			sumMag = std::sqrt(sumMag + 0.5);
			sumStdDev = std::sqrt(sumStdDev + 0.5);

			for (Params& p : vec) {
				//p.mag /= sumMag;			// normalizing the magnitude makes things worse
				p.stdDev /= sumStdDev;
			}

		}


	};

}

#endif // MAXGRADIENT_H
