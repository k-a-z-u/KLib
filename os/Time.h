#ifndef TIME_H
#define TIME_H

#include <cstdint>

#if defined(__linux__)
#include <sys/time.h>
#elif defined(__WIN32__) || defined(_WINDOWS)
#define NOMINMAX
#include <windows.h>
#else
#error "TODO"
#endif

namespace K {

	/**
	 * some helper methods for timing
	 */
	class Time {

	public:

		/** get time in milliseconds */
		static uint64_t getTimeMS() {

#if defined(__linux__)
			struct timeval tv;
			gettimeofday(&tv, NULL);
			return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#elif defined(__WIN32__) || defined(_WINDOWS)
            return GetTickCount();
#else
#error "TODO"
#endif

		}

	};

}

#endif // TIME_H
