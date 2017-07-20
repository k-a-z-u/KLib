#ifndef K_GEO_ELLIPSE_H
#define K_GEO_ELLIPSE_H

#include <vector>

#include "Point2.h"
#include "../Assertions.h"

//#include <eigen3/Eigen/Dense>
//#include "../misc/gnuplot/Gnuplot.h"
//#include "../misc/gnuplot/GnuplotPlot.h"
//#include "../misc/gnuplot/GnuplotPlotElementLines.h"

namespace K {

	class Ellipse {

	public:

		struct CanonicalParams;
		struct GeometricParams;
		struct VectorParams;

		// https://en.wikipedia.org/wiki/Ellipse#General_ellipse
		// http://math.stackexchange.com/questions/1824910/how-to-get-the-coordinates-of-the-center-of-the-ellipse-after-approximation
		struct CanonicalParams {

			// those are quity tricky under some conditions -> double makes sense
			double A;
			double B;
			double C;
			double D;
			double E;
			double F;

			/** empty ctor */
			CanonicalParams() : A(0), B(0), C(0), D(0), E(0), F(1) {
				;
			}

			/** ctor */
			CanonicalParams(const double A, const double B, const double C, const double D, const double E, const double F) : A(A), B(B), C(C), D(D), E(E), F(F) {
				;
			}

			std::string asString() const {
				return std::to_string(A) + "x^2 + " + std::to_string(B) + "xy " + std::to_string(C) + "y^2 + " + std::to_string(D) + "x + " + std::to_string(E) + "y + " + std::to_string(F) + " = 0";
			}

			/** is this a valid ellipse? */
			bool valid() const {
				return (A+B+C+D+E) != 0;	// see empty ctor
			}

			/** ctor with geometric details */
			template <typename Scalar> CanonicalParams(const K::Point2<Scalar> center, const float a, const float b, const float rad) {
				const float cosR = std::cos(rad);
				const float sinR = std::sin(rad);
				A = a*a*sinR*sinR + b*b*cosR*cosR;
				B = 2*(b*b - a*a) * sinR * cosR;
				C = a*a*cosR*cosR  + b*b*sinR*sinR;
				D = -2*A*center.x - B*center.y;
				E = -B*center.x - 2*C*center.y;
				F = A*center.x*center.x + B*center.x*center.y + C*center.y*center.y - a*a*b*b;
				fixF();
				normalize();
				checkMe();
			}

			/** get the ellipse's center */
			K::Point2f getCenter() const {
				const double cx = (2.0*C*D - B*E) / (B*B - 4.0*A*C);
				const double cy = (2.0*A*E - B*D) / (B*B - 4.0*A*C);
				return K::Point2f((float)cx, (float)cy);
			}

			/** get the ellipse's both axis lengths */
			K::Point2f getRadi() const {
				//const double Fx = F + ( D*(B*E - C*D) - A*E*E ) / (4*A*C - B*B);
				//const float a = (float) std::sqrt( (-2*Fx) / (A+C + std::sqrt(B*B + (A-C)*(A-C)) ) );
				//const float b = (float) std::sqrt( (-2*Fx) / std::abs(A+C - std::sqrt(B*B + (A-C)*(A-C)) ) );

				const double a = - std::sqrt(2.0 * (A*E*E + C*D*D - B*D*E + (B*B - 4.0*A*C) * F) * ( A + C + std::sqrt((A-C)*(A-C)+B*B) ) ) / (B*B - 4.0*A*C);
				const double b = - std::sqrt(2.0 * (A*E*E + C*D*D - B*D*E + (B*B - 4.0*A*C) * F) * ( A + C - std::sqrt((A-C)*(A-C)+B*B) ) ) / (B*B - 4.0*A*C);

				// after the calculation, float is enough
				return K::Point2f((float)a, (float)b);

			}

			/** get the ellipse's rotation angle */
			float getAngle() const {
				const double rad = std::atan( (C - A - std::sqrt( (A-C)*(A-C) + B*B )) / B );
				return (float) rad;
			}

			/** get the error for (x,y) not belonging to the ellipse */
			float getError(const float x, const float y) const {

				// very important constraint! otherwise error-calculation fails!
				// // https://en.wikipedia.org/wiki/Ellipse#General_ellipse
				if ((B*B - 4*A*C) >= 0) {return NAN;}

				return (float) (A*x*x + B*x*y + C*y*y + D*x + E*y + F);
				//return A*x*x + B*x*y + C*y*y + fo*(D*x + E*y) + fo*fo*F;

			}

			/** normalize the ellipse-parameters */
			void normalize() {
				const double v = std::sqrt(A*A + B*B + C*C + D*D + E*E + F*F);
				A/=v; B/=v; C/=v, D/=v, E/=v, F/=v;
//				const double v2 = A*A + B*B + C*C + D*D + E*E + F*F;
//				(void) v2;
			}

			void fixF() {
				if (F < 0) {
					A = -A; B = -B; C = -C; D = -D; E = -E; F = -F;
				}
			}



			/** convert to geometric parameters */
			GeometricParams toGeometric() const;

		private:

			// sanity checks (unit-length, correct F factor, ..)
			void checkMe() const {
				const double len = A*A + B*B + C*C + D*D + E*E + F*F;
//				if (len < 0.99 || len > 1.01 || F <= 0.0) {
//					int i = 0;
//				}
				_assertNear(len, 1.0, 0.01, "vector is not normalized!");
				_assertTrue(F > 0, "F must be positive!");
			}

		};



		/**
		 * geometric ellipse description:
		 * center
		 * radius a/b
		 * rotation
		 */
		struct GeometricParams {

			K::Point2f center;

			float a;
			float b;

			float rad;

			/** empty ctor */
			GeometricParams() {;}

			/** ctor */
			GeometricParams(const K::Point2f center, const float a, const float b, const float rad) : center(center), a(a), b(b), rad(rad) {

				// ensure major-axis is > minor axis
				if(b > a) {std::swap(this->a, this->b); this->rad += (float)M_PI_2;}

			}

			/** get a point on this ellipse for the position identified by rad [0:2PI] */
			Point2f getPointFor(const float rad) const {

				// http://stackoverflow.com/questions/2153768/draw-ellipse-and-ellipsoid-in-matlab

				const float cosA = std::cos(rad);			// the point on the circle to draw
				const float sinA = std::sin(rad);

				const float cosB = std::cos(this->rad);		// rotation of the ellipse
				const float sinB = std::sin(this->rad);

				const float x = center.x + (a * cosA * cosB - b * sinA * sinB);
				const float y = center.y + (a * cosA * sinB + b * sinA * cosB);

				return K::Point2f(x,y);

			}

			/** APX circumfence */
			float getCircumfence() const {
				const float h = ((a-b)*(a-b)) / ((a+b)*(a+b));
				return (float)M_PI * (a+b) * ( 1.0f + (3.0f * h) / (10.0f + std::sqrt(4.0f - 3.0f * h)) );
			}

			/** get the ellipse's ratio */
			float getRatio() const {
				return std::max(a,b) / std::min(a,b);
			}

			/** get a new enlarged (>1) or shrunk (<1) version of the ellipse while keeping the center as-is */
			GeometricParams scaled(const float scaler) const {
				return GeometricParams(center, a*scaler, b*scaler, rad);
			}

			Point2f getNearest(const float x, const float y) const {

//				// determine the angle for (x,y) from the center (cx,cy)
//				const float rad = std::atan2(y-center.y, x-center.x);

				Point2f v1(cos(0), sin(0)); v1.normalize();
				Point2f v2(x-center.x, y-center.y); v2.normalize();

				float xrad = (v1.x*v2.x) + (v1.y*v2.y);
				xrad += rad;

				Point2f p1 = getPointFor(xrad);
				Point2f p2 = getPointFor(xrad+(float)M_PI);

				const float d1 = p1.getDistance(Point2f(x,y));
				const float d2 = p2.getDistance(Point2f(x,y));

				return (d1<d2) ? (p1) : (p2);

			}

			/** get the distance of the given point from the ellipse */
			float getDistance(const float x, const float y) const {

				// fetch the corresponding point on the ellipse
				const Point2f pOnEllipse = getNearest(x, y);

				// determine distance
				const float dist = pOnEllipse.getDistance(Point2f(x,y));

				// done
				return dist;


			}

			/** convert to canonical parameters */
			CanonicalParams toCanonical() const;

			/** convert to vector parameters */
			VectorParams toVector() const;

			/** get a difference score between the given ellipse and this ellipse */
			float getDifference(const GeometricParams& other) const {
				const float dCenter = center.getDistance(other.center);
				const float da = std::abs( std::max(a, b) - std::max(other.a, other.b) );
				const float db = std::abs( std::min(a, b) - std::min(other.a, other.b) );
				const float dAxis = std::sqrt(da*da + db*db);
				const float dRad = std::abs(rad - other.rad);
				return dCenter + dAxis + dRad;
			}

			void mix(const GeometricParams& geo, const float mix) {
				this->center =	this->center * (1.0f - mix) + geo.center * mix;
				this->a =		this->a * (1.0f - mix) + geo.a * mix;
				this->b =		this->b * (1.0f - mix) + geo.b * mix;
			}

			/** scale the ellipse while keeping center and rotation */
			GeometricParams operator *= (const float scale) {
				return GeometricParams(center, a*scale, b*scale, rad);
			}

		private:

			void check() const {
				_assertNotNAN(a, "major axis is NaN");
				_assertNotNAN(b, "minor axis is NaN");
				_assertTrue(a >= b, "major axis is smaller than minor axis");
			}

		};


		/**
		 * Point-On-Ellipse = CENTER + u*VEC_MAJOR + v*VEC_MINOR
		 * u = cos(rad);
		 * v = sin(rad);
		 */
		struct VectorParams {

			/** ellipse center */
			Point2f center;

			/** vector for the major axis (including length!) */
			Point2f vecMajor;

			/** vector for the minor axis (including length!) */
			Point2f vecMinor;

			/** ctor */
			VectorParams(const Point2f center, const Point2f major, const Point2f minor) : center(center), vecMajor(major), vecMinor(minor) {;}

			/** get point-on-ellipse given the angle [0:2PI] */
			const Point2f getPointFor(const float rad) const {
				const float u = std::cos(rad);
				const float v = std::sin(rad);
				return center + (vecMajor * u) + (vecMinor * v);
			}

		};


/*
		struct DistanceEstimatorBruteForce {

			std::vector<Point2f> points;

			DistanceEstimatorBruteForce(const GeometricParams& geo) {

				const float stepSize = (float)(M_PI*2) / geo.getCircumfence() * 2;
				for (float i = 0; i < (float)(M_PI*2); i+= stepSize) {
					const Point2f pos = geo.getPointFor(i);
					points.push_back(pos);
				}

			}

			Point2f getNearest(const float x, const float y) const {

				const Point2f p(x,y);
				auto comp = [p] (const Point2f& p1, const Point2f& p2) {
					return p1.getDistance(p) < p2.getDistance(p);
				};

				return *std::min_element(points.begin(), points.end(), comp);

			}

			float getDistance(const float x, const float y) const {
				return getNearest(x,y).getDistance(Point2f(x,y));
			}

		};
*/



		struct DistanceEstimatorBisect {

			float minRadChange;
			const Ellipse::VectorParams params;

			/** ctor */
			DistanceEstimatorBisect(const Ellipse::GeometricParams params) : params(params.toVector()) {

				// circumfence of a circle induced by the major-axis
				const float circ = (float)M_PI * params.a * 2;

				// ~ 0.5 ellipse-pixels in radians (indicator for stopping the refinement)
				minRadChange = (float)(M_PI*2) / circ * 0.33f;

			}

			/** optimize the functions only parameter until epsilon is reached */
			template <typename Func, typename Scalar> void calculateOptimum(const Func& func, Scalar& dst) const {

				struct POS {
					float rad;
					float dist;
					POS(float rad, float dist) : rad(rad), dist(dist) {;}
				};

				// 4 starting points, 0°, 90°, 180°, 270° -> find the best one
				POS pBest(0, 999999);
				for (float rad = 0; rad < (float)(M_PI*2); rad += (float)(M_PI/2)) {
					const POS p (rad, func(rad));
					if (p.dist < pBest.dist) {pBest = p;}
				}

				// analyze region [best-PI/2:best+PI/2]
				// the real value lies somewhere within this regio
				const float pi2 = (float) M_PI_2;
				POS p1(pBest.rad-pi2, func(pBest.rad-pi2));		// p1 = best - PI/2
				POS p2(pBest.rad+pi2, func(pBest.rad+pi2));		// p2 = best + PI/2

				// refine the higher(p2) and lower(p1) bound
				// after some iterations, the changes are already very small
				// and we safely can stop the refinement
				while(true) {

					const float reg = p2.rad - p1.rad;
					const float adv = reg * 0.20f;				// if * x is too high, approximation will fail in some cases!

					// refine the worse one of both
					if (p2.dist > p1.dist) {
						p2.rad -= adv;
						p2.dist = func(p2.rad);
					} else {
						p1.rad += adv;
						p1.dist = func(p1.rad);
					}

					// limit reached?
					if (adv < minRadChange) {break;}

				}

				// done
				dst = (p1.rad+p2.rad) / 2;

			}


			Point2f getNearest(const float x, const float y) const {
				return getNearest(Point2f(x,y));
			}

			Point2f getNearest(const Point2f query) const {

	//			static K::Gnuplot gp;
	//			K::GnuplotPlot plot;
	//			K::GnuplotPlotElementLines lines; plot.add(&lines);
	//			K::GnuplotPlotElementLines test; plot.add(&test);
	//
	//			gp << "set view equal xy\n";
	//			gp << "set xrange [-20:+20]\n";
	//			gp << "set yrange [-20:+20]\n";
	//
	//			for (float f = 0; f < M_PI*2; f += 0.1) {
	//				const float u = std::sin(f);
	//				const float v = std::cos(f);
	//				const Point2f p = params.center + (params.vecMajor*u) + (params.vecMinor*v);
	//				lines.add(K::GnuplotPoint2(p.x, p.y));
	//			}

				auto distance = [&] (const float param) {

					const Point2f p = params.getPointFor(param);

	//				test.clear();
	//				test.add(K::GnuplotPoint2(query.x, query.y));
	//				test.add(K::GnuplotPoint2(p.x, p.y));
	//				gp.draw(plot);
	//				gp.flush();
	//				usleep(1000*100);

					const float dist = p.getDistance(query);
					return dist;

				};

				float param = 0;
				calculateOptimum(distance, param);

				//distance(param);
				//sleep(1);

				return params.getPointFor(param);

			}

		};


	};

	inline Ellipse::GeometricParams Ellipse::CanonicalParams::toGeometric() const {
		checkMe();
		return GeometricParams(getCenter(), getRadi().x, getRadi().y, getAngle());
	}

	inline Ellipse::CanonicalParams Ellipse::GeometricParams::toCanonical() const {
		return CanonicalParams(center, a, b, rad);
	}

	inline Ellipse::VectorParams Ellipse::GeometricParams::toVector() const {
		check();
		const Point2f vec1 = Point2f(1,0).getRotated(rad) * a;		// major axis
		const Point2f vec2 = Point2f(0,1).getRotated(rad) * b;		// minor axis
		return VectorParams(center, vec1, vec2);
	}

}

#endif // K_GEO_ELLIPSE_H
