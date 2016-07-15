#ifndef K_CL_IMAGE_H
#define K_CL_IMAGE_H


#include "Base.h"
#include "ImageParams.h"
#include "Queue.h"
#include "Event.h"
#include "Context.h"
#include "Data.h"

#include <iomanip>

namespace K {
	namespace CL {



		/**
		 * helper class to transfer images and
		 * work with them on within OpenCL
		 */
		class Image {

			CLASS_NAME("Image");

		private:

			ImageDesc desc;
			ImageFormat fmt;

			/** the local data representation */
			Data data;

			/** the buffer's handle */
			cl_mem mem;

		private:

			friend class ImageFactory;

			/** hidden ctor. use ImageFactory */
			Image(const ImageDesc desc, ImageFormat fmt) : desc(desc), fmt(fmt) {;}

		public:

			/** dtor */
			~Image() {
				verboseMeID(mem, "dtor");
				//if (data) {free(data); data = nullptr;}
				clReleaseMemObject(mem); mem = 0;
			}

			/** no-copy */
			Image(const Image& o) = delete;

			/** no-assign */
			Image& operator = (const Image& o) = delete;

			/** get the image's handle */
			cl_mem getHandle() const {
				return mem;
			}

		public:

			/** transfer the image's content from the host to the device. returns an event to wait for */
			Event upload(Queue* queue) {
				verboseMeID(mem, "upload");
				cl_event event;
				const size_t origin[] = {0,0,0};
				const size_t region[] = {desc.image_width, desc.image_height, desc.image_depth};
				check( clEnqueueWriteImage(queue->getHandle(), getHandle(), CL_FALSE, origin, region, desc.image_row_pitch, desc.image_slice_pitch, data.getData(), 0, nullptr, &event) );
				return Event(event);
			}

			/** transfer the image's content from the device back to the host. returns an event to wait for */
			Event download(Queue* queue) {
				verboseMeID(mem, "download");
				cl_event event;
				const size_t origin[] = {0,0,0};
				const size_t region[] = {desc.image_width, desc.image_height, desc.image_depth};
				check( clEnqueueReadImage(queue->getHandle(), getHandle(), CL_FALSE, origin, region, desc.image_row_pitch, desc.image_slice_pitch, data.getData(), 0, nullptr, &event) );
				return Event(event);
			}


			/** get the underlying data */
			const Data& getData() {return data;}


		};

	}
}


#endif // K_CL_IMAGE_H
