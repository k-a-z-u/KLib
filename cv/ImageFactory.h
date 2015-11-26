#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H

#include "ImageChannel.h"

#include <png.h>
#include <jpeglib.h>

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



		static ImageChannel readJPEG(const std::string& file) {

			struct jpeg_decompress_struct cinfo;
			struct jpeg_error_mgr jerr;
			const int buffer_height = 1;

			FILE* fp = fopen(file.c_str(), "rb");
			if (!fp) {throw "error";}

			cinfo.err = jpeg_std_error(&jerr);
			jpeg_create_decompress(&cinfo);
			jpeg_stdio_src(&cinfo, fp);
			(void) jpeg_read_header(&cinfo, true);
			(void) jpeg_start_decompress(&cinfo);

			// number of bytes per line
			const int row_stride = cinfo.output_width * cinfo.output_components;

			// allocate line-buffer
			JSAMPARRAY buffer = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * buffer_height);
			buffer[0] = (JSAMPROW)malloc(sizeof(JSAMPLE) * row_stride);

			// the to-be-constructed output image
			ImageChannel img(cinfo.output_width, cinfo.output_height);

			// read all lines of the image
			int row = 0;
			while (cinfo.output_scanline < cinfo.output_height) {

				// read one line
				jpeg_read_scanlines(&cinfo, (JSAMPARRAY)buffer, 1);

				// convert pixel to GREY and set it within the output image
				for (int x = 0; x < (int)cinfo.output_width; ++x) {
					const float val = (buffer[0][x*cinfo.output_components]) / 255.0f;
					img.set(x,row,val);
				}

				// next
				++row;

			}

			// cleanup
			(void) jpeg_finish_decompress(&cinfo);
			jpeg_destroy_decompress(&cinfo);
			free(buffer[0]);
			free(buffer);
			fclose(fp);

			// done
			return img;

		}

	};

	}

#endif // IMAGEFACTORY_H

