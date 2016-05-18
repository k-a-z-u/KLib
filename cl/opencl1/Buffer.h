#ifndef K_CL_BUFFER_H
#define K_CL_BUFFER_H

#include "Base.h"
#include "Queue.h"
#include "Event.h"
#include "Context.h"

#include <iomanip>

namespace K {
	namespace CL {

		class Buffer {

		private:

			/** the context the buffer belongs to */
			//Context* ctx;

			/** the local data representation */
			void* data = nullptr;

			size_t dataSize = 0;

			/** the buffer's handle */
			cl_mem mem;

			/** buffer is readable? */
			bool readable;

			/** buffer is writeable? */
			bool writeable;

		public:

			/** create a buffer, the kernel is allowed to read only */
			static Buffer* createReadOnly(Context* ctx, const void* data, const size_t size) {
				Buffer* buf = new Buffer();
				cl_int status;
				cl_mem_flags memFlags = CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR;
				buf->dataSize = size;
				buf->readable = true;
				buf->writeable = false;
				buf->mem = clCreateBuffer(ctx->getHandle(), memFlags, size, (void*) data, &status);
				check(status);
				return buf;
			}

			/** create a buffer, the kernel is allowed to read only */
			static Buffer* createWriteOnly(Context* ctx, const size_t size) {
				Buffer* buf = new Buffer();
				cl_int status;
				cl_mem_flags memFlags = CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR;
				buf->data = (uint8_t*) malloc(size);
				buf->dataSize = size;
				buf->readable = false;
				buf->writeable = true;
				buf->mem = clCreateBuffer(ctx->getHandle(), memFlags, size, buf->data, &status);
				check(status);
				return buf;
			}

			//		/** create a buffer, the kernel is allowed to read only */
			//		static Buffer* createReadWrite(Context* ctx, const size_t size) {
			//			Buffer* buf = new Buffer();
			//			cl_int status;
			//			cl_mem_flags memFlags = CL_MEM_READ_WRITE;// | CL_MEM_USE_HOST_PTR;
			//			buf->data = malloc(size);
			//			buf->mem = clCreateBuffer(ctx->getID(), memFlags, size, data, &status);
			//			check(status);
			//			return buf;
			//		}

		private:

			/** hidden ctor. use static methods */
			Buffer() {;}

		public:

			/** dtor */
			~Buffer() {
				if (data) {free(data); data = nullptr;}
				clReleaseMemObject(mem); mem = 0;
			}

			/** no-copy */
			Buffer(const Buffer& o) = delete;

			/** no-assign */
			Buffer& operator = (const Buffer& o) = delete;

		public:

			/** does this buffer support reading? */
			bool supportsRead() const {return readable;}

			/** does this buffer support writing? */
			bool supportsWrite() const {return writeable;}



			/** transfer the buffer's content from the host to the device. returns an event to wait for */
			Event upload(Queue* queue) {
				if (!supportsRead()) {return Event(0);}
				const size_t offset = 0;
				cl_event event;
				check( clEnqueueWriteBuffer(queue->getHandle(), getHandle(), CL_FALSE, offset, getSize(), getHostPointer(), 0, nullptr, &event) );
				return std::move(Event(event));
			}

			/** transfer the buffer's content from the device back to the host. returns an event to wait for */
			Event download(Queue* queue) {
				if (!supportsWrite()) {return Event(0);}
				const size_t offset = 0;
				cl_event event;
				check( clEnqueueReadBuffer(queue->getHandle(), getHandle(), CL_FALSE, offset, getSize(), getHostPointer(), 0, nullptr, &event) );
				return std::move(Event(event));
			}

			void dumpHEX(const size_t perLine = 8) {
				for (size_t i = 0; i < dataSize; ++i) {
					std::cout << std::setw(2);
					std::cout << std::setfill('0');
					std::cout << std::hex << (int) ((uint8_t*)data)[i] << ' ';
					if (i % 8 == (perLine-1)) {std::cout << std::endl;}
				}
				std::cout << std::endl;
			}

			void dumpINT(const size_t perLine = 8) {
				for (size_t i = 0; i < dataSize; ++i) {
					std::cout << std::setw(3);
					std::cout << std::setfill('0');
					std::cout << (int) ((uint8_t*)data)[i] << ' ';
					if (i % 8 == (perLine-1)) {std::cout << std::endl;}
				}
				std::cout << std::endl;
			}



			/** get the buffer's handle */
			cl_mem getHandle() const {
				return mem;
			}

			/** get the buffer's size */
			size_t getSize() const {
				return dataSize;
			}

			/** get the pointer of the host's data-area */
			uint8_t* getHostPointer() {
				return (uint8_t*) data;
			}

		};

	}
}

#endif // K_CL_BUFFER_H
