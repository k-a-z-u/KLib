#ifndef K_CV_KERNELFACTORYIMPL_H
#define K_CV_KERNELFACTORYIMPL_H

#include "Program.h"
#include "Kernel.h"

#include <algorithm>

namespace K {
	namespace CL {

		inline KernelFactory::KernelFactory(Program* prg) : prg(prg) {
			;
		}

		inline KernelFactory::~KernelFactory() {
			for (Kernel* ker : kernels) {delete ker;}
		}

		inline Kernel* KernelFactory::create(const std::string& methodName) {
			Kernel* ker = new Kernel(prg, methodName);
			kernels.push_back(ker);
			return ker;
		}

		inline void KernelFactory::destroy(Kernel* kernel) {
			kernels.erase(std::find(kernels.begin(), kernels.end(), kernel));
			delete kernel;
		}

		inline Kernel* KernelFactory::get(const int idx) {
			return kernels[idx];
		}

	}
}

#endif // K_CV_KERNELFACTORYIMPL_H
