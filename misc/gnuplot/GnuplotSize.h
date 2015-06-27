#ifndef GNUPLOTSIZE_H
#define GNUPLOTSIZE_H

namespace K {

	struct GnuplotSize {

	public:

		const double width;

		const double height;


	public:

		GnuplotSize(const double w, const double h) : width(w), height(h) {;}

		double getWidth(const std::string& term) {
			if		(term == "wxt" || term == "svg" || term == "pngcairo") {return width * 30;}
			else					{return width / 2.54;}		// to inch
		}

		double getHeight(const std::string& term) {
			if		(term == "wxt" || term == "svg" || term == "pngcairo") {return height * 30;}
			else					{return height / 2.54;}		// to inch
		}

	};

}

#endif // GNUPLOTSIZE_H
