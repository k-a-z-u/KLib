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


			/** create a kernel for the given method name. all created programs will be free-ed on factory destruction */
			Kernel* create(const std::string& methodName);

			/** manually destroy the given kernel. do NOT use the pointer hereafter */
			void destroy(Kernel* prg);

		};

	}
}

#endif // K_CV_KERNELFACTORY_H
