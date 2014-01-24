#ifndef MEMORYUSAGE_H
#define MEMORYUSAGE_H

#if defined(__linux__)
#include <malloc.h>
#include <sys/sysinfo.h>
#elif defined(__WIN32__)
#include <windows.h>
#include <psapi.h>
#else
error "TODO"
#endif

namespace K {

	/**
	 * get details about a process's memory usage
     *
     * windows:
     *      needs linking against "-lpsapi"
     *
	 */
	class MemoryUsage {

	public:

		/** get the amount of used memory (in bytes) of this process */
		static uint64_t getProcessUsedMemory() {

#if defined(__linux__)
			struct mallinfo m = mallinfo();
			return (uint64_t)m.arena;
#elif defined(__WIN32__)
            PROCESS_MEMORY_COUNTERS mem;
            GetProcessMemoryInfo(GetCurrentProcess(), &mem, sizeof(mem));
            return mem.WorkingSetSize;
#else
#endif

		}



		/** get the total amount of memory on this machine */
		static uint64_t getTotalMemory() {

#if defined(__linux__)
			struct sysinfo info;
			sysinfo(&info);
			return info.totalram;
#elif defined(__WIN32__)
            MEMORYSTATUSEX statex;
            statex.dwLength = sizeof(statex);
            GlobalMemoryStatusEx(&statex);
            return statex.ullTotalPhys;
#else
#endif

		}

	};

}

#endif // MEMORYUSAGE_H
