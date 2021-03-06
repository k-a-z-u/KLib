#ifndef IMAGEPARAMS_H
#define IMAGEPARAMS_H

#include "Base.h"

namespace K {
	namespace CL {

		/**
		 * supported images formats
		 */
		enum class ImageFormat {

			RGBA_UINT8_255,
			RGBA_UINT8_1,
			RGBA_UINT16_1,
			RGBA_FLOAT,

			GRAY_UINT8,
			GRAY_UINT16,
			GRAY_FLOAT,

		};

		/**
		 * image description
		 */
		struct ImageDesc : public cl_image_desc {

			/** 2D image */
			ImageDesc(const int width, const int height) {
				image_type = CL_MEM_OBJECT_IMAGE2D;
				image_width = width;
				image_height = height;
				image_depth = 1;		// single 2D image
				image_array_size = 0;
				image_row_pitch = 0;	// no stride
				image_slice_pitch = 0;	// just simple 2D
				num_mip_levels = 0;
				num_samples = 0;
				buffer = nullptr;
			}

			/** 3D Image */
			ImageDesc(const int width, const int height, const int depth) {
				image_type = CL_MEM_OBJECT_IMAGE3D;
				image_width = width;
				image_height = height;
				image_depth = depth;	// multiple 2D images
				image_array_size = 0;
				image_row_pitch = 0;	// no stride
				image_slice_pitch = 0;	// just simple 2D
				num_mip_levels = 0;
				num_samples = 0;
				buffer = nullptr;
			}

		};

	}
}

#endif // IMAGEPARAMS_H
