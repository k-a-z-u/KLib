#ifndef OPENCLBASE_H
#define OPENCLBASE_H

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS		// we do not yet use openCL 2.0
//#define __NO_STD_VECTOR // Use cl::vector instead of STL version
#include <CL/cl.h>

namespace CL {

	#define check(res) _check(res, __FILE__, __LINE__, __FUNCTION__)

	/** check the given result code and terminate in case of errors */
	static inline void _check(const cl_int res, const char* file, const int line, const char* func) {
		if (res != CL_SUCCESS) {
			std::cerr << "error in '" << func << "()' [" << file << ", line " << line << "] code: " << res << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	static inline void warn(const std::string msg) {
		std::cout << "WARN: " << msg << std::endl;
	}

}

#endif // OPENCLBASE_H
