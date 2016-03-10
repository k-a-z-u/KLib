#ifndef GNUPLOTSIZE_H
#define GNUPLOTSIZE_H

namespace K {

	struct GnuplotSize {

	public:

		const float width;

		const float height;


	public:

		GnuplotSize(const float w, const float h) : width(w), height(h) {;}

		float getWidth(const std::string& term) {
			if		(term == "wxt" || term == "svg" || term == "pngcairo") {return width * 30.0f;}
			else					{return width / 2.54f;}		// to inch
		}

		float getHeight(const std::string& term) {
			if		(term == "wxt" || term == "svg" || term == "pngcairo") {return height * 30.0f;}
			else					{return height / 2.54f;}		// to inch
		}

	};

}

#endif // GNUPLOTSIZE_H
