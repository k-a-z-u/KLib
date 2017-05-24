#ifndef GNUPLOTCOLOR_H
#define GNUPLOTCOLOR_H

#include <string>

namespace K {

	/** coloring */
	class GnuplotColor {

	private:

		std::string colorDesc;

		/** empty ctor: black */
		GnuplotColor(const std::string& desc) : colorDesc(desc) {
			;
		}


	public:

		/** empty ctor: black */
		GnuplotColor() : colorDesc("rgb '#000000'") {
			;
		}

		/** set from hex-string: e.g. "#0000ff" */
		void setHexStr(const std::string& hexStr) {
			colorDesc = "rgb '" + hexStr + "'";
		}

		/** let gnuplot decide */
		void setAuto() {
			colorDesc = "";
		}

		/** from RGB */
		static GnuplotColor fromRGB(int r, int g, int b) {
			GnuplotColor c;
			c.colorDesc.resize(7);
			r = clamp(r, 0, 255);
			g = clamp(g, 0, 255);
			b = clamp(b, 0, 255);
			sprintf((char*)c.colorDesc.data(), "#%02x%02x%02x", r, g, b);
			c.colorDesc = "rgb '" + c.colorDesc + "'";
			return c;
		}

		static GnuplotColor fromHSV(int h, int s, int v) {

			unsigned char region, remainder, p, q, t;
			int r, g, b;

			// gray?
			if (s == 0) {
				return fromRGB(v,v,v);
			}

			region = h / 43;
			remainder = (h - (region * 43)) * 6;

			p = (v * (255 - s)) >> 8;
			q = (v * (255 - ((s * remainder) >> 8))) >> 8;
			t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

			switch (region) {
				case 0:
					r = v; g = t; b = p; break;
				case 1:
					r = q; g = v; b = p; break;
				case 2:
					r = p; g = v; b = t; break;
				case 3:
					r = p; g = q; b = v; break;
				case 4:
					r = t; g = p; b = v; break;
				default:
					r = v; g = p; b = q; break;
			}

			return fromRGB(r,g,b);

		}

		/** from hex-string : "#00ff00" */
		static GnuplotColor fromHexStr(const std::string& hex) {
			return GnuplotColor("rgb '" + hex + "'");
		}

		/** let gnuplot decide the color */
		static GnuplotColor AUTO() {
			return GnuplotColor("");
		}

		/** is the color configured to let gnuplot choose? */
		bool isAuto() const {
			return colorDesc == "";
		}

		/** get in gnuplot format */
		std::string toGP() const {
			return colorDesc;
		}



	private:

		static inline int clamp(const int val, const int min, const int max) {
			if (val < min) {return min;}
			if (val > max) {return max;}
			return val;
		}

	};

}

#endif // GNUPLOTCOLOR_H
