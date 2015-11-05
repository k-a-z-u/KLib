#ifndef K_LOG_LOGGERSTDOUT_H
#define K_LOG_LOGGERSTDOUT_H

#include "Logger.h"
#include <mutex>

namespace K {

	class LoggerStdOut : public Logger {

	private:
		std::mutex mtx;

	public:

		void add(const uint64_t ts, const std::string& component, const LogLevel lvl, const std::string& msg) override {
			mtx.lock();
			std::cout << '[' << ts << "][" << component << "] " << levelToString(lvl) <<	": " << msg << std::endl;
			mtx.unlock();
		}

	};

}

#endif // LOGGERSTDOUT_H
