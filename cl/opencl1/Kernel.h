#ifndef KERNEL_H
#define KERNEL_H

#include "Base.h"
#include "Program.h"
#include "Buffer.h"
#include "Event.h"

#include <string>

namespace CL {

	class Kernel {

	private:

		/** the program the kernel belongs to */
		Program* program;

		/** the name of the kernel (= method name within the program) */
		std::string kernelName;

		/** the created kernel handle */
		cl_kernel kernel;

	public:

		/** ctor */
		Kernel(Program* program, const std::string& kernelName) : program(program), kernelName(kernelName) {
			init();
		}

		/** attach the given buffer to the kernel's n-th argument */
		void setArg(const cl_uint argIdx, Buffer* buf) {
			cl_mem mem = buf->getID();
			cl_int res = clSetKernelArg(kernel, argIdx, sizeof(mem), &mem);
			check(res);
		}

		/** copy the given integer into the kernel's n-th argument */
		void setArg(const cl_uint argIdx, const cl_int val) {
			cl_int res = clSetKernelArg(kernel, argIdx, sizeof(val), &val);
			check(res);
		}

		/** execute the kernel. async. returns an event to wait for */
		Event run() {
			size_t globalWorkSize = 1;
			size_t localWorkSize = 1;
			cl_event event;
			cl_command_queue queue = program->getContext()->getCommandQueues()[0];
			check( clEnqueueNDRangeKernel(queue, kernel, 1, nullptr, &globalWorkSize, &localWorkSize, 0, nullptr, &event) );
			return Event(event);
		}

		/** transfer the given buffer's content from the device back to the host.a sync. returns an event to wait for */
		Event fetch(Buffer* buf) {
			cl_command_queue queue = program->getContext()->getCommandQueues()[0];
			const size_t offset = 0;
			cl_event event;
			check( clEnqueueReadBuffer(queue, buf->getID(), CL_FALSE, offset, buf->getSize(), buf->getHostPointer(), 0, nullptr, &event) );
			return Event(event);
		}

	private:

		/** initialize the kernel */
		void init() {
			cl_int status = 0;
			kernel = clCreateKernel(program->getID(), kernelName.c_str(), &status);
			check(status);
		}

	};

}

#endif // KERNEL_H
