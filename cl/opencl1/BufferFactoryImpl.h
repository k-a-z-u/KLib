#ifndef K_CL_BUFFERFACTORYIMPL_H
#define K_CL_BUFFERFACTORYIMPL_H

#include "Context.h"
#include "BufferFactory.h"

#include <algorithm>

namespace K {
	namespace CL {

		inline BufferFactory::BufferFactory(Context* ctx) : ctx(ctx) {
			;
		}

		inline BufferFactory::~BufferFactory() {
			for (Buffer* buf : buffers) {delete buf;}
		}

		inline void BufferFactory::dispose(Buffer* buf) {
			buffers.erase(std::find(buffers.begin(), buffers.end(), buf));
			delete buf;
		}

		/** create a buffer, the kernel is allowed to read only */
		inline Buffer* BufferFactory::createReadOnly(const void* data, const size_t size) {

			// memory access modes (kernel / host)
			cl_mem_flags memFlags = 0;
			memFlags |= getKernelAccess(DataMode::READ_ONLY);
			memFlags |= getHostAccess(DataMode::WRITE_ONLY);

			// always good?
			memFlags |= CL_MEM_COPY_HOST_PTR;

			Buffer* buf = new Buffer();
			buffers.push_back(buf);

			cl_int status;
			buf->data = Data(data, size, false);
			buf->mem = clCreateBuffer(ctx->getHandle(), memFlags, size, (void*) buf->data.getData(), &status);
			check(status);
			return buf;
		}

		/** create a buffer, the kernel is allowed to read only */
		inline Buffer* BufferFactory::createWriteOnly(const size_t size) {

			// memory access modes (kernel / host)
			cl_mem_flags memFlags = 0;
			memFlags |= getKernelAccess(DataMode::WRITE_ONLY);
			memFlags |= getHostAccess(DataMode::READ_ONLY);

			// always good?
			memFlags |= CL_MEM_COPY_HOST_PTR;

			Buffer* buf = new Buffer();
			buffers.push_back(buf);

			cl_int status;
			buf->data = Data(size);
			buf->mem = clCreateBuffer(ctx->getHandle(), memFlags, size, buf->data.getData(), &status);
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



	}
}

#endif // K_CL_BUFFERFACTORYIMPL_H
