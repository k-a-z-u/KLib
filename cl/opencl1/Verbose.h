#ifndef K_CL_VERBOSE_H
#define K_CL_VERBOSE_H

#ifdef WITH_VERBOSE
#include <string>
#include <iostream>
#endif

namespace K {
	namespace CL {

#ifdef WITH_VERBOSE
		#define CLASS_NAME(name) const char* me = name;
		#define verboseMe(msg) std::cout << "[" << me << this << "] " << msg << std::endl;
		#define verboseMeID(id, msg) std::cout << "[" << me << this << "(" << id << ")] " << msg << std::endl;
#else
		#define CLASS_NAME(name)
		#define verboseMe(msg)
		#define verboseMeID(id, msg)
#endif

	}
}

#endif // K_CL_VERBOSE_H
