#ifndef K_GEO_ELLIPSEDISTANCE_H
#define K_GEO_ELLIPSEDISTANCE_H

#include "Ellipse.h"
#include "Point2.h"

namespace K {

	namespace EllipseDistance {

		/**
		 * to reduce the complexity of calculations
		 * center (0,0) and axis-align (remove rotation) the ellipse.
		 * do the same for query-points.
		 * find the best match using region splitting (use 3 points on the ellipse, refine the worst of the 3)
		 * translate the result back to the original ellipse
		 */
		class AlignedSplit {

		private:

			const float deg0 = 0;
			const float deg45 = (float)(M_PI/4);
			const float deg90 = (float)(M_PI/2);

		private:

			/** estimation quality. 10 is already very good */
			const int numIterations;

			/** original (unaligned) ellipse params */
			const Ellipse::GeometricParams parOrig;

			/** axis aligned ellipse params */
			struct Aligned {

				const float a;		// length of the major axis;
				const float b;		// length of the minor axis;

				/** ctor */
				Aligned(const float a, const float b) : a(a), b(b) {;}

				/** get the position (x,y) for the given angle */
				Point2f getPointFor(const float rad) const {
					const float x = std::cos(rad) * a;
					const float y = std::sin(rad) * b;
					return Point2f(x,y);
				}

			} aligned;

			/** helper class for faster result calculation */
			struct Result {

				float alignedRad;
				int origQuadrant;
				float distance;

				/** convert from aligned-radians [0:PI/2] (1st quadrant only) to unaligned radians [0:2PI], depending on the original quadrant */
				float getOrigRad() const {
					const float deg180 = (float) M_PI;
					if (origQuadrant == 0) {return +alignedRad;}
					if (origQuadrant == 1) {return deg180-alignedRad;}
					if (origQuadrant == 2) {return deg180+alignedRad;}
					if (origQuadrant == 3) {return -alignedRad;}
					throw 1;
				}

			};

		public:

			/** ctor */
			AlignedSplit(const Ellipse::GeometricParams params, const int numIterations = 10) :  numIterations(numIterations), parOrig(params), aligned(params.a, params.b) {
				;
			}

			/** get a point on the ellipse, nearest to (x,y) */
			Point2f getBestPoint(const float x, const float y) const {
				return getBestPoint(Point2f(x,y));
			}

			/** get a point on the ellipse, nearest to (x,y) */
			Point2f getBestPoint(const Point2f query) const {

				// get the best result within Quadrant Q1 and re-translate everything to the original ellipse
				const Result res = getBest(query);

				// position on original ellipse including the correct quadrant
				//return getOrig(res.alignedRad, res.origQuadrant);
				return parOrig.getPointFor(res.getOrigRad());

			}

			/** just get the distance of the given query-point to the ellipse. FASTER than getNearest()! */
			float getBestDistance(const float x, const float y) const {
				return getBestDistance(Point2f(x,y));
			}

			/** just get the distance of the given query-point to the ellipse. FASTER than getNearest()! */
			float getBestDistance(const Point2f query) const {
				return _getBest(query).distance;
			}

			Result getBest(const Point2f query) const {
				return _getBest(query);
			}

			Result getBest(const float x, const float y) const {
				return getBest(Point2f(x,y));
			}

		private:


			/** get a point on the ellipse, nearest to (x,y) */
			Result _getBest(const Point2f _query) const {

				// translate the point into the coordinate-system of the axis-aligned ellipse (center and undo rotation)
				const Point2f query = (_query - parOrig.center).getRotated(-parOrig.rad);

				// quadrant for the new query point?
				const int q = getQuadrant(query);

				// now ensure the query-points belongs to quadrant Q1 [=0] by using its absolute(x,y)
				const Point2f aQuery = query.getAbs();


//				static K::Gnuplot gp;
//				K::GnuplotPlot plot;
//				K::GnuplotPlotElementLines lines; plot.add(&lines);
//				K::GnuplotPlotElementLines test; plot.add(&test);
//				gp << "set view equal xy\n";
//				for (float f = 0; f < M_PI*2; f += 0.1) {
//					const Point2f p = aligned.getPointFor(f);
//					lines.add(K::GnuplotPoint2(p.x, p.y));
//				}
//				for (float f = 0; f < M_PI*2; f += 0.1) {
//					const Point2f p = parOrig.getPointFor(f);
//					lines.add(K::GnuplotPoint2(p.x, p.y));
//				}


				// 3 points on the Q1 ellipse
				Splitter splt;
				splt.add( POS(deg0,		dst(deg0,	aQuery)) );
				splt.add( POS(deg45,	dst(deg45,	aQuery)) );
				splt.add( POS(deg90,	dst(deg90,	aQuery)) );

				// refine-loop that replaces the worst of the 3 points above
				for (int i = 0; i < numIterations; ++i) {

//					test.clear();
//					K::GnuplotPoint2 gp1(aQuery.x, aQuery.y);
//					const Point2f pA = aligned.getPointFor(splt.p1.rad);
//					const Point2f pB = aligned.getPointFor(splt.p2.rad);
//					const Point2f pC = aligned.getPointFor(splt.p3.rad);
//					test.addSegment(gp1, K::GnuplotPoint2(pA.x, pA.y));
//					test.addSegment(gp1, K::GnuplotPoint2(pB.x, pB.y));
//					test.addSegment(gp1, K::GnuplotPoint2(pC.x, pC.y));

//					K::GnuplotPoint2 gp2(_query.x, _query.y);
//					const Point2f pD = getOrig(splt.nextRad(), q);
//					test.addSegment(gp2, K::GnuplotPoint2(pD.x, pD.y));

//					gp.draw(plot);
//					gp.flush();
//					usleep(1000*50);

					// calculate the error for the position between best and 2nd-best
					const float nextRad = splt.nextRad();
					const POS next(nextRad, dst(nextRad, aQuery));
					splt.add(next);

				}

				// done
				Result res;
				res.origQuadrant = q;
				res.distance = splt.p1.dist;
				res.alignedRad = splt.nextRad();	// even a little better than p1 ?
				return res;

			}

		private:


			/** get distance between pointFor(alignedRad) and alignedQuery */
			float dst(const float alignedRad, const Point2f alignedQuery) const {
				return aligned.getPointFor(alignedRad).getDistance(alignedQuery);
			}

			/** get the quadrant [0:3] the given point belongs to */
			int getQuadrant(const Point2f p) const {
				if (p.x >= 0) {
					return (p.y >= 0) ? (0) : (3);
				} else {
					return (p.y >= 0) ? (1) : (2);
				}
			}

//			/** remove the alignment and add the original quadrant */
//			Point2f getOrig(const float alignedRad, const int quadrant) const {
//				const float deg180 = (float) M_PI;
//				if (quadrant == 0) {return parOrig.getPointFor(+alignedRad);}
//				if (quadrant == 1) {return parOrig.getPointFor(deg180-alignedRad);}
//				if (quadrant == 2) {return parOrig.getPointFor(deg180+alignedRad);}
//				if (quadrant == 3) {return parOrig.getPointFor(-alignedRad);}
//				throw 1;
//			}

			/** position on ellipse + its distance (=error) from the query point */
			struct POS {
				float rad;
				float dist;
				POS(const float rad, const float dist) : rad(rad), dist(dist) {;}
			};

			/**
			 * magic happens here
			 * keep 3 points on the ellipse
			 * the worst one is replaced by the center of the best and 2nd best
			 */
			struct Splitter {

				POS p1 = POS(0, 999999999);		// best
				POS p2 = POS(0, 999999999);		// 2nd
				POS p3 = POS(0, 999999999);		// 3rd

				/** add the given point according to its error */
				void add(const POS& p) {
					if (p.dist < p1.dist) {
						p3 = p2; p2 = p1; p1 = p;
					} else if (p.dist < p2.dist) {
						p3 = p2; p2 = p;
					} else if (p.dist < p3.dist) {
						p3 = p;
					}
				}

				/** get the center between best and 2nd best */
				float nextRad() const {
					return (p1.rad + p2.rad) / 2;
				}

			};

		};


	}

}

#endif // K_GEO_ELLIPSEDISTANCE_H
