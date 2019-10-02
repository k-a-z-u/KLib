#ifndef K_CL_BASE_H
#define K_CL_BASE_H

#ifdef WITH_OPENCL

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS		// we do not yet use openCL 2.0
//#define __NO_STD_VECTOR // Use cl::vector instead of STL version
#include <CL/cl.h>
#include <iostream>

#include "../../Exception.h"
#include "Errors.h"
#include "Verbose.h"
#include "Constants.h"

namespace K {
	namespace CL {

		#define check(res) _check(res, __FILE__, __LINE__, __FUNCTION__)

		/** check the given result code and terminate in case of errors */
		static inline void _check(const cl_int res, const char* file, const int line, const char* func) {
			if (res != CL_SUCCESS) {
				std::cerr << "error in '" << func << "()' [" << file << ", line " << line << "] code: " << res << " msg: " << Errors::get(res) << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		static inline void warn(const std::string msg) {
			std::cout << "WARN: " << msg << std::endl;
		}


		/** supported modes of data-access */
		enum class DataMode {
			READ_ONLY,
			WRITE_ONLY,
			READ_WRITE,
		};

		/** convert the given DataMode to a memory flag indicating what the KERNEL does with the data  */
		static inline cl_mem_flags getKernelAccess(const DataMode mode) {
			switch (mode) {
				case DataMode::READ_ONLY:	return CL_MEM_READ_ONLY;
				case DataMode::WRITE_ONLY:	return CL_MEM_WRITE_ONLY;
				case DataMode::READ_WRITE:	return CL_MEM_READ_WRITE;
			}
			throw "should not happen";
		}

		/** convert the given DataMode to a memory flag indicating what the HOST does with the data  */
		static inline cl_mem_flags getHostAccess(const DataMode mode) {
			switch (mode) {
				case DataMode::READ_ONLY:	return CL_MEM_HOST_READ_ONLY;
				case DataMode::WRITE_ONLY:	return CL_MEM_HOST_WRITE_ONLY;
				case DataMode::READ_WRITE:	return 0;
			}
			throw "should not happen";
		}

	}
}

#endif

#endif // K_CL_BASE_H
