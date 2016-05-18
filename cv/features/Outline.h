#ifndef K_CV_FEATURES_OUTLINE_H
#define K_CV_FEATURES_OUTLINE_H

#include "../ImageChannel.h"
#include "../Point2.h"

#include "../../math/filter/MovingAVG.h"

#include <vector>
#include <fstream>

namespace K {

	/** the detected outline */
	struct Outline {

		/** all points that are part of the outline */
		std::vector<Point2i> points;

	};

	/**
	 * follow the outline of a (binary) image starting
	 * at a given seed-point by walking along the line.
	 */
	class OutlineDetection {

		enum Dir {
			UP,
			RIGHT,
			DOWN,
			LEFT,
		};

		struct State {

			/** current heading (up,down,left,right) */
			Dir curDir;

			/** current position */
			Point2i curPos;

			/** ctor */
			State(const Dir dir, const Point2i pos) : curDir(dir), curPos(pos) {;}

			State walkLeft()	{return go((Dir)((curDir + 3) % 4)); }
			State walkRight()	{return go((Dir)((curDir + 1) % 4)); }
			State walkBack()	{return go((Dir)((curDir + 2) % 4)); }
			State walkForward()	{return go(curDir);}

		private:

			State go(const Dir d) {
				switch (d) {
					case UP:	return State(d, curPos + Point2i( 0, -1));
					case RIGHT:	return State(d, curPos + Point2i(+1,  0));
					case DOWN:	return State(d, curPos + Point2i( 0, +1));
					case LEFT:	return State(d, curPos + Point2i(-1,  0));
					default:	throw 1;
				}
			}

		};

	private:


		Dir curDir = LEFT;

	public:

		/** follow the outline given by seed */
		static Outline follow(const K::ImageChannel& img, const Point2i seed, const float threshold = 0.5) {

			State state(Dir::DOWN, seed);
			Outline outline;

			// start at the seed
			state.curPos = seed;

			// walk
			while (true) {

				// done?
				if (!outline.points.empty() && outline.points.front() == state.curPos) {
					break;
				}

				// point is OK
				outline.points.push_back(state.curPos);

				// try to walk right
				if (tryToGo(img, threshold, state, state.walkRight())) {continue;}

				// try to walk straight
				if (tryToGo(img, threshold, state, state.walkForward())) {continue;}

				// try to go left
				if (tryToGo(img, threshold, state, state.walkLeft())) {continue;}

				// walk backward
				if (tryToGo(img, threshold, state, state.walkBack())) {continue;}


			}

			return outline;

		}

	private:

		static bool tryToGo(const K::ImageChannel& img, const float threshold, State& curState, const State tryState) {
			if (isOK(img, curState.curPos, tryState.curPos, threshold)) {
				curState = tryState;
				std::cout << tryState.curPos.asString() << std::endl;
				return true;
			}
			return false;
		}

		static bool isOK(const K::ImageChannel& img, const Point2i cur, const Point2i next, const float threshold) {
			const float v1 = img.get(cur.x, cur.y);
			const float v2 = img.get(next.x, next.y);
			return std::abs(v1-v2) < threshold;
		}

	};

	class OutlineProcessing {

	public:

		/** find all edge-points within the given outline */
		static std::vector<Point2i> getEdges(const Outline& l) {

			const int regionSize = 3;
			const int stepSize = 10;
			const int num = (int) l.points.size();


			// sanity check (otherwise: bad results)
			if (stepSize < 3) {throw Exception("invalid step-size given. must be at least 3");}

			std::ofstream o("/tmp/1.dat");

			std::vector<float> values;

			// add all angular changes
			for (int i = 0; i < num; ++i) {

				// center, left-of and right-of point
				Point2i pl = l.points[(i - stepSize + num) % num];
				Point2i p0 = l.points[i];
				Point2i pr = l.points[(i + stepSize) % num];

				// vector from center->left, center->right
				Point2i v1 = pl - p0;
				Point2i v2 = p0 - pr;		// so both vectors point in the same direction => no change = 0 degree angle

				// dot-product and vector length to get the angle between
				const float l1 = std::sqrt(v1.x*v1.x + v1.y*v1.y);
				const float l2 = std::sqrt(v2.x*v2.x + v2.y*v2.y);
				const float angle = std::acos( (v1.x*v2.x + v1.y*v2.y) / l1 / l2 );
				values.push_back(angle);
				o << angle << std::endl;

			}

			// find maxima within angular changes
			for (int i = 0; i < num; ++i) {
				if (values[i] >= 0.5 && isLocalMaxima(values, i, regionSize)) {
					std::cout << "+" << i << std::endl;
					std::cout << l.points[i].asString() << std::endl;
				}
			}

			o.close();

			std::vector<Point2i> edges;
			return edges;

		}


		/** is the given index a local maxima within the given region? */
		template <typename T> static bool isLocalMaxima(const T& src, const int idx, const int size) {

			const int num = (int) src.size();

			// number of equal values on the left/right
			int lEQ = 0;
			int rEQ = 0;

			// window to check
			for (int i = -size; i <= size; ++i) {
				if (i == 0) {continue;}
				const int _idx = (idx + i + num) % num;
				if (src[_idx] > src[idx]) {return false;}
				if (src[_idx] == src[idx]) {
					++((i < 0) ? lEQ : rEQ);
				}
			}

			// number of equal values is the same on the left/right
			if (lEQ - rEQ == 0) {return true;}

			// number of equal values is one more on the right
			if (lEQ - rEQ == -1) {return true;}

			return false;

		}

	};

}

#endif // K_CV_FEATURES_OUTLINE_H
