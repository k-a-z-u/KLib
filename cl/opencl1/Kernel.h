#ifndef K_CL_KERNEL_H
#define K_CL_KERNEL_H

#include "Base.h"
#include "Buffer.h"
#include "Image.h"
#include "Event.h"
#include "Range.h"
#include "Program.h"
#include "Queue.h"

#include <string>

namespace K {
	namespace CL {

		class KernelFactory;

		class Kernel {

			CLASS_NAME("Kernel");

		private:

			/** the program the kernel belongs to */
			Program* program;

			/** the name of the kernel (= method name within the program) */
			std::string kernelName;

			/** the created kernel handle */
			cl_kernel kernel;

		private:

			friend class KernelFactory;

			/** ctor */
			Kernel(Program* program, const std::string& kernelName) : program(program), kernelName(kernelName) {
				verboseMe("ctor");
				init();
			}

		public:

			/** dtor */
			~Kernel() {
				verboseMeID(kernel, "dtor");
				clReleaseKernel(kernel); kernel = 0;
			}

			/** no-copy */
			Kernel(const Kernel& o) = delete;

			/** no-assign */
			Kernel& operator = (const Kernel& o) = delete;

		public:

			/** get the program the kernel belongs to */
			Program* getProgram() const {
				return program;
			}

			/** attach the given buffer to the kernel's n-th argument */
			void setArg(const cl_uint argIdx, Buffer* buf) {
				cl_mem mem = buf->getHandle();
				cl_int res = clSetKernelArg(kernel, argIdx, sizeof(mem), &mem);
				check(res);
			}

			/** attach the given image to the kernel's n-th argument */
			void setArg(const cl_uint argIdx, Image* img) {
				cl_mem mem = img->getHandle();
				cl_int res = clSetKernelArg(kernel, argIdx, sizeof(mem), &mem);
				check(res);
			}


			/** copy the given integer into the kernel's n-th argument */
			void setArg(const cl_uint argIdx, const cl_int val) {
				cl_int res = clSetKernelArg(kernel, argIdx, sizeof(val), &val);
				check(res);
			}

			/** execute the kernel. async. returns an event to wait for */
			Event run(Queue* queue, int dimensions, Range global, Range local) {
				cl_event event;
				check( clEnqueueNDRangeKernel(queue->getHandle(), kernel, dimensions, nullptr, (size_t*) &global, (size_t*) &local, 0, nullptr, &event) );
				return std::move(Event(event));
			}

		private:

			/** initialize the kernel */
			void init() {
				cl_int status = 0;
				kernel = clCreateKernel(program->getHandle(), kernelName.c_str(), &status);
				check(status);
			}

		};

	}
}

#endif // K_CL_KERNEL_H
