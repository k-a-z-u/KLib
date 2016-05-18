#ifndef BUFFER_H
#define BUFFER_H

#include "Base.h"
#include "Context.h"

#include <iomanip>


namespace CL {

	class Buffer {

	private:

		/** the context the buffer belongs to */
		Context* ctx;

		/** the local data representation */
		void* data = nullptr;

		size_t dataSize = 0;

		/** the buffer's handle */
		cl_mem mem;

	public:

//		/** ctor */
//		Buffer(Context* ctx, const size_t size) {
//			data.resize(size);
//			cl_int status;
//			cl_mem_flags memFlags = CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR;
//			mem = clCreateBuffer(ctx->getID(), memFlags, size, data.data(), &status);
//			check(status);
//		}

		~Buffer() {
			if (data) {free(data); data = nullptr;}
		}

		/** create a buffer, the kernel is allowed to read only */
		static Buffer* createReadOnly(Context* ctx, const void* data, const size_t size) {
			Buffer* buf = new Buffer();
			cl_int status;
			cl_mem_flags memFlags = CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR;
			buf->dataSize = size;
			buf->mem = clCreateBuffer(ctx->getID(), memFlags, size, (void*) data, &status);
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
			buf->mem = clCreateBuffer(ctx->getID(), memFlags, size, buf->data, &status);
			check(status);
			return buf;
		}

		void dumpHEX() {
			for (size_t i = 0; i < dataSize; ++i) {
				std::cout << std::setw(2);
				std::cout << std::setfill('0');
				std::cout << std::hex << (int) ((uint8_t*)data)[i] << ' ';
				if (i % 8 == 7) {std::cout << std::endl;}
			}
			std::cout << std::endl;
		}

		void dumpINT() {
			for (size_t i = 0; i < dataSize; ++i) {
				std::cout << std::setw(3);
				std::cout << std::setfill('0');
				std::cout << (int) ((uint8_t*)data)[i] << ' ';
				if (i % 8 == 7) {std::cout << std::endl;}
			}
			std::cout << std::endl;
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

		/** get the buffer's handle */
		cl_mem getID() const {
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
#endif // BUFFER_H
