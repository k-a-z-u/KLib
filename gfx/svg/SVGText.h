#ifndef SVGTEXT_H_
#define SVGTEXT_H_


#include "SVGElement.h"
#include "../../geo/Line.h"
#include "../Color.h"


namespace K {

	class SVGText : public SVGElement {

	public:

		/** ctor */
		SVGText() {;}

		void transform(const double x, const double y) override {
			pos.x += x;
			pos.y += y;
		}

		/** get the text's position */
		const Point getPosition() const {return pos;}

		/** get the text's string */
		const std::string& getText() const {return text;}

		virtual SVGElementType getType() const override {
			return SVGElementType::TEXT;
		}

	private:


		friend class SVGLoader;

		/** the text's anchor position */
		Point pos;

		/** the textual content */
		std::string text;

	};

}

#endif //SVGTEXT_H_

