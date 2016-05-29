#ifndef K_CV_KERNELFACTORY_H
#define K_CV_KERNELFACTORY_H

#include <vector>
#include <string>

namespace K {
	namespace CL {

		class Kernel;
		class Program;

		/**
		 * create data-buffers that belong to a context
		 */
		class KernelFactory {

		private:

			/** the program the kernel belongs to */
			Program* prg;

			/** all kernels created by the factory */
			std::vector<Kernel*> kernels;

		private:

			friend class Program;

			/** hidden ctor */
			KernelFactory(Program* prg);

		public:

			/** dtor */
			~KernelFactory();

			/** create a kernel for the given method name */
			Kernel* create(const std::string& methodName);

		};

	}
}

#endif // K_CV_KERNELFACTORY_H
