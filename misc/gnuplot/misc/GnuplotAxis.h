#ifndef GNUPLOTAXIS_H
#define GNUPLOTAXIS_H

#include "../GnuplotStructs.h"
#include "GnuplotStringMod.h"
#include <string>

namespace K {

	class GnuplotAxis {

	public:

		/** struct for min/max range */
		struct Range {

			static constexpr float AUTO = NAN;
			float min;
			float max;

			/** empty ctor: auto scale */
			Range() : min(AUTO), max(AUTO) {;}

			/** ctor with values */
			Range(float min, float max) : min(min), max(max) {;}

			/** manual or auto scaling? */
			bool isManual() const {return min==min || max==max;}

			/** as gnuplot string */
			std::string toGP() const {

				std::string res = "[";
				if (min!=min) {res += '*';} else {res += std::to_string(min);}
				res += ":";
				if (max!=max) {res += '*';} else {res += std::to_string(max);}
				res += ']';
				return res;

			}

		};

		//static constexpr float TICS_STEP_AUTO = NAN;

	private:

		/** name within gnuplot commands [e.g. x, x2, y, y2, z, cb] */
		std::string typeName;

		/** is the axis visible? */
		bool showTics = true;
		float ticsOffX = 0;
		float ticsOffY = 0;
		//float ticsStep = TICS_STEP_AUTO;	// step betwenn tics [NAN = auto]
		std::string ticsStep = "autofreq";
		std::string ticsLabelFormat = "%h";

		/** the axis' label */
		std::string label;
		float labelOffX = 0;
		float labelOffY = 0;
		int labelRot_deg = 0;

		/** the axis's range */
		Range range;

		friend class GnuplotPlot;
		friend class GnuplotSplot;

		/** hidden ctor */
		GnuplotAxis(const std::string typeName, const bool showTics) : typeName(typeName), showTics(showTics) {
			;
		}

	public:

		/** whether to show tics or not */
		void setTicsVisible(const bool show) {
			this->showTics = show;
		}

		/** move the axis' tics from their default position */
		void setTicsOffset(const float x, const float y) {
			this->ticsOffX = x;
			this->ticsOffY = y;
		}

		/** automatic tics stepping */
		void setTicsStepAuto() {
			this->ticsStep = "autofreq";
		}

		/** set the step-size between adjacent tics */
		void setTicsStep(const float incr) {
			this->ticsStep = std::to_string(incr);
		}

		/** set tics between [start:end] using the given increment */
		void setTicsStep(const float start, const float incr, const float end) {
			this->ticsStep = std::to_string(start) + ", " + std::to_string(incr) + ", " + std::to_string(end);
		}

		/** configure manual tics for the given positions */
		void setTicsStep(std::vector<float> positions) {
			this->ticsStep = "(";
			for (size_t i = 0; i < positions.size(); ++i) {
				if (i > 0) {this->ticsStep += ",";}
				this->ticsStep += std::to_string(positions[i]);
			}
			this->ticsStep += ") ";
		}

		/** set the format for the tics-labels (default: "%h") */
		void setTicsLabelFormat(const std::string& format) {
			this->ticsLabelFormat = format;
		}


		/** set the axis' label */
		void setLabel(const std::string& label) {
			this->label = label;
		}

		/** move the axis' label */
		void setLabelOffset(const float x, const float y) {
			this->labelOffX = x;
			this->labelOffY = y;
		}

		/** allows rotating the label by the given amout */
		void setLabelRotation(const int deg) {
			this->labelRot_deg = deg;
		}

		/** set the axis' range */
		void setRange(const Range r) {
			this->range = r;
		}

		/** set the axis' range */
		void setRange(const float min, const float max) {
			this->range = Range(min,max);
		}

	public:

		/** conver to gnuplot commands */
		void addTo(std::ostream& ss, const GnuplotStringMod* mod) const {

			// range?		e.g. set x2range [0:10]
			if (range.isManual())	{
				ss << "set " << typeName << "range " << range.toGP() << "\n";
			}

			// tics?		e.g. set x2tics
			if (showTics) {
				ss << "set " << typeName << "tics ";
				if (ticsStep == ticsStep) {ss << " " << ticsStep << " ";}
				if (ticsOffX != 0 || ticsOffY != 0)		{ss << " offset " << ticsOffX << "," << ticsOffY << " ";}
				ss << "\n";
				ss << "set format " << typeName << " '" << mod->modTics(ticsLabelFormat) << "'\n";
			} else {
				ss << "unset " << typeName << "tics \n";
			}


			// label?
			if (!label.empty())	{
				ss << "set " << typeName << "label '" << mod->modLabel(label) << "' ";
				if (labelRot_deg != 0)					{ss << " rotate by " << labelRot_deg;}
				if (labelOffX != 0 || labelOffY != 0)	{ss << " offset " << labelOffX << "," << labelOffY << " ";}
				ss << "\n";
			} else {
				ss << "unset " << typeName << "label \n";
			}

		}

	};

}

#endif // GNUPLOTAXIS_H
