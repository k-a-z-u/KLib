#ifndef K_CV_IMAGEFACTORY_H
#define K_CV_IMAGEFACTORY_H

#include <vector>
#include <cstdint>

#include "ImageParams.h"

namespace K {
	namespace CL {

		class Image;
		class Context;

		/**
		 * create images that belong to a context
		 */
		class ImageFactory {

		private:

			/** the context the BufferFactory belongs to */
			Context* ctx;

			/** all images created by the factory */
			std::vector<Image*> images;

		private:

			friend class Context;

			/** hidden ctor */
			ImageFactory(Context* ctx);

		public:

			/** dtor */
			~ImageFactory();

			/** create a read-only buffer */
			Image* createReadOnly(const ImageDesc desc, const ImageFormat fmt, const uint8_t* data, const size_t len);

			/** create a write-only buffer */
			Image* createWriteOnly(const ImageDesc desc, const ImageFormat fmt, uint8_t* data, const size_t len);

			/** manually delete the given image */
			void dispose(Image* img);

		private:

			/** convert our ImageFormat enum to an openCL image-format-struct */
			static inline cl_image_format getImageFormat(const ImageFormat fmt) {
				switch(fmt) {

				case ImageFormat::RGBA_UINT8_255:	return getImageFormat(CL_RGBA, CL_UNSIGNED_INT8);		// input: [0:255] output: SAME! access via read_imagei/read_imageui

				case ImageFormat::RGBA_UINT8_1:		return getImageFormat(CL_RGBA, CL_UNORM_INT8);			// input: [0:255] output: [0.0:1.0]. acccess via read_imagef
				case ImageFormat::RGBA_UINT16_1:	return getImageFormat(CL_RGBA, CL_UNORM_INT16);
				case ImageFormat::RGBA_FLOAT:		return getImageFormat(CL_RGBA, CL_FLOAT);				// input: [*.*] (all allowed!) output: SAME!. access via read_imagef

				case ImageFormat::GRAY_UINT8:		return getImageFormat(CL_INTENSITY, CL_UNORM_INT8);
				case ImageFormat::GRAY_UINT16:		return getImageFormat(CL_INTENSITY, CL_UNORM_INT16);
				case ImageFormat::GRAY_FLOAT:		return getImageFormat(CL_INTENSITY, CL_FLOAT);

				}

				throw "unsupported image format";

			}

			/** helper method for image-format-struct setup */
			static inline cl_image_format getImageFormat(cl_channel_order order, cl_channel_type type) {
				cl_image_format fmt;
				fmt.image_channel_order = order;
				fmt.image_channel_data_type = type;
				return fmt;
			}

		};

	}
}

#endif // K_CV_IMAGEFACTORY_H
