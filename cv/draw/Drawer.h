#ifndef K_CV_DRAWER_H
#define K_CV_DRAWER_H

#include "../ImageChannel.h"
#include "../../geo/Point2.h"
#include "Fonts.h"
#include "../../geo/BBox2.h"
#include "../../geo/Ellipse.h"
#include "../filter/Interpolation.h"

namespace K {

	/**
	 * very simple helper to draw common structures onto an ImageChannel
	 */
	class Drawer {

	private:

		ImageChannel& img;
		float fg = 0.0f;
		float bg = 1.0f;

		Font fnt = Font::getDefault();

	public:

		/** ctor */
		Drawer(ImageChannel& img) : img(img) {

		}

		/** change the foreground color */
		void setForeground(const float fg) {
			this->fg = fg;
		}


		/** set the font to use */
		void setFont(const Font& font) {
			this->fnt = font;
		}

		void drawString(const std::string& str, const Point2i& pos) {

			Point2i p = pos;
			for (char c : str) {
				const Glyph g = fnt.getGlyph(c);
				drawImage(g, p);
				p.x += g.getWidth();
			}

		}

		/** draw the given image at the provided position */
		void drawImage(const K::ImageChannel& img, const Point2i& dst, const float transparent = NAN) {
			drawImage<K::ImageChannel>(img, dst, transparent);
		}

		/** draw the given image at the provided position */
		template <typename Elem> void drawImage(const Elem& img, const Point2i& dst, const float transparent = NAN) {

			for (int y = 0; y < img.getHeight(); ++y) {
				for (int x = 0; x < img.getWidth(); ++x) {
					const int x1 = dst.x + x;
					const int y1 = dst.y + y;
					if (isOutside(x1, y1)) {continue;}
					const float val = img.get(x,y);
					if (val == transparent) {continue;}
					this->img.set(x1, y1, val);
				}
			}

		}

		/**
		 * draw the given image at the provided position
		 * the mixer determines the value to write after receiving the current and the new pixel value
		 */
		template <typename Mix> void drawImage(const K::ImageChannel& img, const Point2i& dst, Mix mix) {

			for (int y = 0; y < img.getHeight(); ++y) {
				for (int x = 0; x < img.getWidth(); ++x) {
					const int x1 = dst.x + x;
					const int y1 = dst.y + y;
					if (isOutside(x1, y1)) {continue;}
					const float newVal = img.get(x,y);				// value within the new image
					const float oldVal = this->img.get(x1,y1);		// value within the current image (to overwrite)
					const float val = mix(oldVal, newVal);			// determine mixing value
					this->img.set(x1, y1, val);						// set
				}
			}

		}

		/** draw the given dot */
		void drawDot(const float x, const float y) {
			Interpolation::Bilinear::set(img, x, y, fg);
		}

		/** draw an ellipse */
		void drawEllipse(const Ellipse::GeometricParams& params) {
			const float circ = params.getCircumfence();
			const float stepSize = (float)(M_PI*2) / circ * 0.85f;
			for (float rad = 0; rad < (float) M_PI*2; rad += stepSize) {
				const Point2f p = params.getPointFor(rad);
				if (isOutside((int)p.x, (int)p.y, 1)) {continue;}
				//img.set((int)p.x, (int)p.y, fg);
				Interpolation::Bilinear::set(img, p.x, p.y, fg);
			}
		}

		/** draw a rectangle */
		void drawRect(const int x1, const int y1, const int x2, const int y2) {

			drawLine(x1, y1, x2, y1);	// upper
			drawLine(x1, y2, x2, y2);	// lower

			drawLine(x1, y1, x1, y2);	// left
			drawLine(x2, y1, x2, y2);	// right

		}

		/** draw a rectangle */
		void drawRect(const Point2i p1, const Point2i p2) {
			drawRect(p1.x, p1.y, p2.x, p2.y);
		}

		/** fill everything that is NOT part of the rect */
		void fillRectI(int x1, int y1, const int x2, const int y2) {
			BBox2i bbox(x1,y1, x2,y2);
			for (int y = 0; y < img.getHeight(); ++y) {
				for (int x = 0; x < img.getWidth(); ++x) {
					if (bbox.contains(x,y)) {continue;}
					this->img.set(x,y,bg);
				}
			}
		}

		/** draw a line */
		void drawLine(const K::Point2f p1, const K::Point2f p2) {
			drawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y);
		}

		/** draw a line */
		void drawLine(int x1, int y1, const int x2, const int y2) {

			const int dx =  std::abs(x2-x1), sx = (x1<x2 ? 1 : -1);
			const int dy = -std::abs(y2-y1), sy = (y1<y2 ? 1 : -1);
			int err = dx+dy, e2;

			while(1) {

				if (x1 >= 0 && y1 >= 0 && x1 < img.getWidth() && y1 < img.getHeight()) { img.set(x1, y1, fg); }
				if ( (x1==x2) && (y1==y2) ) {break;}
				e2 = 2*err;
				if (e2 > dy) { err += dy; x1 += sx; }
				if (e2 < dx) { err += dx; y1 += sy; }
			}

		}

	private:

		template <typename T> inline bool isOutside(const T x, const T y, const int frame = 0) const {
			if (x < frame)						{return true;}
			if (y < frame)						{return true;}
			if (x >= img.getWidth() - frame)	{return true;}
			if (y >= img.getHeight() - frame)	{return true;}
			return false;
		}

		template <typename T> inline bool isOutside(const Point2<T>& p, const int frame = 0) const {
			return isOutside(p.x, p.y, frame);
		}

	};

}

#endif // K_CV_DRAWER_H
