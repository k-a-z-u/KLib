#ifndef GNUPLOTATTRCOLOR_H
#define GNUPLOTATTRCOLOR_H

class GnuplotAttrColor {

protected:

	std::string color = "rgb '#000000'";

public:

	/** set the color to use for drawing this element using a hex-value */
	void setColorHex(const std::string& hex) {this->color = "rgb '" + hex + "'";}

};

#endif // GNUPLOTATTRCOLOR_H
