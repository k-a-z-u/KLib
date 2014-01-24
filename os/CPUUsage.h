#ifndef CPUUSAGE_H
#define CPUUSAGE_H

#include "Time.h"

#if defined(__linux__)
#include <sys/times.h>
#elif defined(__WIN32__)
#include <windows.h>
#else
#error "TODO"
#endif

#include <iostream>

namespace K {

	class CPUUsageException : std::exception {
	public:
		CPUUsageException() {;}
	};


	/**
	 * get details of a process's cpu usage
	 */
	class CPUUsage {

	public:

		/** get the cpu usage since the last call to this method */
		float getUsageSinceLastCall() {

#if defined(__linux__)
			uint64_t time = Time::getTimeMS();
            std::cout << time << std::endl;
			clock_t clk = clock();

			clock_t clkDiff = clk - last.clk;
			uint64_t timeDiff = time - last.time;

			last.clk = clk;
			last.time = time;

			return float(clkDiff) * 1000.0f / float(CLOCKS_PER_SEC) / float(timeDiff) ;
#elif defined(__WIN32__)

            // current "timestamp"
            uint64_t time = Time::getTimeMS();

            // usage times
            FILETIME fdummy;
            FILETIME fuser;
            FILETIME fkernel;
            GetProcessTimes(GetCurrentProcess(), &fdummy, &fdummy, &fkernel, &fuser);

            // convert to real timings
            SYSTEMTIME user;
            SYSTEMTIME kernel;
            FileTimeToSystemTime(&fuser, &user);
            FileTimeToSystemTime(&fkernel, &kernel);

            // sum up to total cpu time in milliseconds
            uint64_t usage = 0;
            usage += user.wMilliseconds + user.wSecond * 1000 + user.wMinute * 1000 * 60 + user.wHour * 1000 * 60 * 60;
            usage += kernel.wMilliseconds + kernel.wSecond * 1000 + kernel.wMinute * 1000 * 60 + kernel.wHour * 1000 * 60 * 60;

            // delta
            uint64_t diffUsage = usage - last.usage;
            uint64_t diffTime = time - last.time;

            // update last
            last.usage = usage;
            last.time = time;

            return float(diffUsage) / float(diffTime);

#endif

		}

	private:


#if defined(__linux__)
		struct {
			clock_t clk;
			uint64_t time;
		} last;
#elif defined(__WIN32__)
        struct {
            uint64_t usage;
            uint64_t time;
        } last;
#endif

	};

}

#endif // CPUUSAGE_H
