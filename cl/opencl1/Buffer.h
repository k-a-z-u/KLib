#ifndef K_CL_BUFFER_H
#define K_CL_BUFFER_H

#include "Base.h"
#include "Queue.h"
#include "Event.h"
#include "Data.h"
#include "Context.h"
#include "BufferFactory.h"

namespace K {
	namespace CL {

		class Buffer {

			CLASS_NAME("Buffer");

		private:

			friend class BufferFactory;

			/** the local data representation */
			Data data;

			/** the buffer's handle */
			cl_mem mem;

		private:

			/** hidden ctor. use static methods */
			Buffer() {;}

		public:

			/** dtor */
			~Buffer() {
				verboseMeID(mem, "dtor");
				clReleaseMemObject(mem); mem = 0;
			}

			/** no-copy */
			Buffer(const Buffer& o) = delete;

			/** no-assign */
			Buffer& operator = (const Buffer& o) = delete;

		public:

			/** transfer the buffer's content from the host to the device. returns an event to wait for */
			Event upload(Queue* queue) {
				const size_t offset = 0;
				cl_event event;
				check( clEnqueueWriteBuffer(queue->getHandle(), getHandle(), CL_FALSE, offset, getSize(), getHostPointer(), 0, nullptr, &event) );
				return std::move(Event(event));
			}

			/** transfer the buffer's content from the device back to the host. returns an event to wait for */
			Event download(Queue* queue) {
				const size_t offset = 0;
				cl_event event;
				check( clEnqueueReadBuffer(queue->getHandle(), getHandle(), CL_FALSE, offset, getSize(), getHostPointer(), 0, nullptr, &event) );
				return std::move(Event(event));
			}





			/** get the buffer's handle */
			cl_mem getHandle() const {
				return mem;
			}

			/** get the underlying data */
			const Data& getData() {return data;}



		private:

			/** get the pointer of the host's data-area */
			uint8_t* getHostPointer() {
				return (uint8_t*) data.getData();
			}

			/** get the buffer's size */
			size_t getSize() const {
				return data.getSize();
			}

		};

	}
}

#endif // K_CL_BUFFER_H
