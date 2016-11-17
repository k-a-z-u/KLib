#ifndef K_CV_ELLIPSEESTIMATOR_H
#define K_CV_ELLIPSEESTIMATOR_H

#include <cmath>

#include "../../geo/BBox2.h"

//#include "../../misc/gnuplot/Gnuplot.h"
//#include "../../misc/gnuplot/GnuplotPlot.h"
//#include "../../misc/gnuplot/GnuplotPlotElementLines.h"
//#include "../../misc/gnuplot/GnuplotPlotElementPoints.h"


#include "EllipseEstimatorMath.h"
#include "EllipseEstimatorPixel.h"
#include "EllipseEstimatorPixelRANSAC.h"
#include "EllipseEstimatorRANSAC.h"

/*

		template <typename Scalar> static Ellipse::CanonicalParams getRemoveWorst(const std::vector<Point2<Scalar>>& _points) {

			std::vector<Point2<Scalar>> points = _points;
			Ellipse::CanonicalParams params;
			const int toRemove = points.size() * 0.15;
			const int removePerRun = points.size() * 0.04;

			for (int i = 0; i < toRemove/removePerRun; ++i) {

				// estimate params from a random sample-set
				params = get<Scalar>(points);

				// sort points by error
				auto comp = [&] (const Point2<Scalar>& p1, const Point2<Scalar>& p2) { return (params.getError(p1.x,p1.y) > params.getError(p2.x,p2.y)); };
				std::sort(points.begin(), points.end(), comp);

				// remove the worst 2.5%
				points.resize(points.size() - removePerRun);


			}

			return params;

		}


*/

#endif // K_CV_ELLIPSEESTIMATOR_H
