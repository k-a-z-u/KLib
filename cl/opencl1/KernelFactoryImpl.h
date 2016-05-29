#ifndef K_CV_KERNELFACTORYIMPL_H
#define K_CV_KERNELFACTORYIMPL_H

#include "Program.h"
#include "Kernel.h"

namespace K {
	namespace CL {

		KernelFactory::KernelFactory(Program* prg) : prg(prg) {
			;
		}

		KernelFactory::~KernelFactory() {
			for (Kernel* ker : kernels) {delete ker;}
		}

		Kernel* KernelFactory::create(const std::string& methodName) {
			Kernel* ker = new Kernel(prg, methodName);
			kernels.push_back(ker);
			return ker;
		}

	}
}

#endif // K_CV_KERNELFACTORYIMPL_H
