#ifndef GNUPLOTATTRCUSTOM_H
#define GNUPLOTATTRCUSTOM_H

#include <string>

class GnuplotAttrCustom {

protected:

	/** custom attributes. default: empty */
	std::string attrCustom;

public:

	/** set the custom attribute string to use for drawing this element */
	void setCustomAttr(const std::string& attr) {this->attrCustom = attr;}

	/** get the current custom attribute string */
	const std::string& getCustomAttr() {return this->attrCustom;}

};

#endif // GNUPLOTATTRCUSTOM_H
