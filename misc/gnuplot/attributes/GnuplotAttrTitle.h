#ifndef GNUPLOTATTRTITLE_H
#define GNUPLOTATTRTITLE_H

#include <string>

class GnuplotAttrTitle {

protected:

	std::string title;

public:

	/** set the title to use for this element */
	void setTitle(const std::string& title) {
		this->title = title;
	}

};

#endif // GNUPLOTATTRTITLE_H
