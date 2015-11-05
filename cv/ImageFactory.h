#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H

#include "ImageChannel.h"

#include <png.h>
#include <string>

namespace K {

	class ImageFactory {

	public:

		static DataMatrix<uint8_t> to8(const ImageChannel& img) {
			DataMatrix<uint8_t> out(img.getWidth(), img.getHeight());
			const int num = img.getWidth() * img.getHeight();
			for (int i = 0; i < num; ++i) {
				out.getData()[i] = (uint8_t) ( ((ImageChannel&)img).getData()[i] * 255 );
			}
			return out;
		}

		static void writePNG(const std::string& file, const ImageChannel& _img) {

			DataMatrix<uint8_t> img = to8(_img);

			png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if (!png) {throw "Could not allocate write struct\n";}

			png_infop info_ptr = png_create_info_struct(png);
			if (!info_ptr) {throw "error 2";}

			if (setjmp(png_jmpbuf(png))) {throw "error 3";}

			const int w = img.getWidth();
			const int h = img.getHeight();

			// configure
			png_set_IHDR(png, info_ptr, w, h, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

			// create an array containing the start of each row within the image
			uint8_t* rows[h];
			for (int y = 0; y < h; ++y) { rows[y] = &img.getData()[y*w]; }
			png_set_rows(png, info_ptr, &rows[0]);

			// write
			FILE* fp = fopen(file.c_str(), "wb");
			png_init_io (png, fp);
			png_write_png(png, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
			fclose(fp);

		}

	};

}

#endif // IMAGEFACTORY_H

