#ifndef K_CL_QUEUE_H
#define K_CL_QUEUE_H

#include "Base.h"

namespace K {
	namespace CL {

	class Queue {

		private:

			/** the openCL queue handle */
			cl_command_queue queue;

		private:

			friend class Context;

			/** ctor with OpenCL queue handle */
			Queue(cl_command_queue queue) : queue(queue) {
				;
			}

		public:

			/** dtor */
			~Queue() {
				clReleaseCommandQueue(queue);
			}

			/** no-copy */
			Queue(const Queue& o) = delete;

			/** no-assign */
			Queue& operator = (const Queue& o) = delete;

		public:

			/** get the OpenCL handle */
			cl_command_queue getHandle() const {
				return queue;
			}

		};

	}
}

#endif // K_CL_QUEUE_H
