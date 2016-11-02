#ifndef K_CV_IMAGEFACTORYIMPL_H
#define K_CV_IMAGEFACTORYIMPL_H

#include "Image.h"
#include "ImageParams.h"
#include "ImageFactory.h"

#include <algorithm>

namespace K {
	namespace CL {

		inline ImageFactory::ImageFactory(Context* ctx) : ctx(ctx) {
			;
		}

		inline ImageFactory::~ImageFactory() {
			for (Image* img : images) {delete img;}
		}

		inline void ImageFactory::destroy(Image* img) {
			images.erase(std::find(images.begin(), images.end(), img));
			delete img;
		}

		/** create a new image, readable by the kernel, writeable by the host */
		inline Image* ImageFactory::createReadOnly(const ImageDesc desc, const ImageFormat fmt, const uint8_t* data, const size_t len) {

			// memory access modes (kernel / host)
			cl_mem_flags memFlags = 0;
			memFlags |= getKernelAccess(DataMode::READ_ONLY);
			memFlags |= getHostAccess(DataMode::WRITE_ONLY);

			// always good?
			memFlags |= CL_MEM_COPY_HOST_PTR;

			// create
			Image* img = new Image(desc, fmt);
			images.push_back(img);

			cl_int status;
			cl_image_format format = getImageFormat(fmt);
			img->data = Data(data, len);
			img->mem = clCreateImage(ctx->getHandle(), memFlags, &format, &desc, (void*) img->data.getData(), &status);
			check(status);

			// done
			return img;

		}

		/** create a new image, writeable by the kernel, readable by the host */
		inline Image* ImageFactory::createWriteOnly(const ImageDesc desc, const ImageFormat fmt, uint8_t* data, const size_t len) {

			// memory access modes (kernel / host)
			cl_mem_flags memFlags = 0;
			memFlags |= getKernelAccess(DataMode::WRITE_ONLY);
			memFlags |= getHostAccess(DataMode::READ_ONLY);

			// always good?
			memFlags |= CL_MEM_COPY_HOST_PTR;

			// create
			Image* img = new Image(desc, fmt);
			images.push_back(img);

			cl_int status;
			cl_image_format format = getImageFormat(fmt);
			img->data = Data(data, len);
			img->mem = clCreateImage(ctx->getHandle(), memFlags, &format, &desc, (void*) img->data.getData(), &status);
			check(status);

			// done
			return img;

		}



	}
}

#endif // K_CV_IMAGEFACTORYIMPL_H
