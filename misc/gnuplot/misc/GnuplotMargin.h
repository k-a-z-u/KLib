#ifndef GNUPLOTMARGIN_H
#define GNUPLOTMARGIN_H


#include <string>

namespace K {

	/** describes the plot's key */
	class GnuplotMargin {

	private:

		float left = NAN;
		float right = NAN;
		float top = NAN;
		float bottom = NAN;

	public:

		/** configure the margins */
		void set(const float left, const float right, const float top, const float bottom) {
			this->left = left;
			this->right = right;
			this->top = top;
			this->bottom = bottom;
		}

		void setLeft(const float left) {
			this->left = left;
		}

		void setRight(const float right) {
			this->right = right;
		}

		void setTop(const float top) {
			this->top = top;
		}

		void setBottom(const float bottom) {
			this->bottom = bottom;
		}

	public:

		/** convert to gnuplot commands */
		void addTo(std::stringstream& ss) const {
			if (left == left)		{ss << "set lmargin " << left << "\n";}		else {ss << "unset lmargin\n";}
			if (right == right)		{ss << "set rmargin " << right << "\n";}	else {ss << "unset rmargin\n";}
			if (top == top)			{ss << "set tmargin " << top << "\n";}		else {ss << "unset tmargin\n";}
			if (bottom == bottom)	{ss << "set bmargin " << bottom << "\n";}	else {ss << "unset bmargin\n";}
		}

	};

}


#endif // GNUPLOTMARGIN_H
