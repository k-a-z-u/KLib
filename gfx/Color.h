/*
 * Color.h
 *
 *  Created on: Dec 20, 2013
 *      Author: kazu
 */

#ifndef COLOR_H_
#define COLOR_H_

#define TO_HEX_DIGIT(val)		(	(((val) < 10)	? ('0'+(val))		: ('A'+(val)-10))	)
#define FROM_HEX_DIGIT(val)		(	(((val)>='A'))	? ((val)-'A'+10)	: ((val)-'0')	)

#include <algorithm>

namespace K {

struct Color {

	/** get as hex string */
	std::string getHex() {
		std::string ret = "#";
		ret += (char) TO_HEX_DIGIT( (r >> 4) & 0xF );
		ret += (char) TO_HEX_DIGIT( (r >> 0) & 0xF );
		ret += (char) TO_HEX_DIGIT( (g >> 4) & 0xF );
		ret += (char) TO_HEX_DIGIT( (g >> 0) & 0xF );
		ret += (char) TO_HEX_DIGIT( (b >> 4) & 0xF );
		ret += (char) TO_HEX_DIGIT( (b >> 0) & 0xF );
		return ret;
	}

	/** set color from hex */
	void setFromHex(const std::string& hex) {
		std::string tmp = hex;
		std::transform(tmp.begin(), tmp.end(),tmp.begin(), ::toupper);
		r = (FROM_HEX_DIGIT(tmp[1]) << 4) + FROM_HEX_DIGIT(tmp[2]);
		g = (FROM_HEX_DIGIT(tmp[3]) << 4) + FROM_HEX_DIGIT(tmp[4]);
		b = (FROM_HEX_DIGIT(tmp[5]) << 4) + FROM_HEX_DIGIT(tmp[6]);
	}

	int r;
	int g;
	int b;

};

}


#endif /* COLOR_H_ */
