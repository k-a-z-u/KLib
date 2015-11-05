#ifndef K_LOG_LOGGERDUMMY_H
#define K_LOG_LOGGERDUMMY_H

#include "Logger.h"

namespace K {

	class LoggerDummy : public Logger {

		void add(const std::string& component, const LogLevel lvl, const std::string& msg) override {
			;
		}

		void add(const uint64_t ts, const std::string& component, const LogLevel lvl, const std::string& msg) override {
			;
		}

	};

}

#endif // K_LOG_LOGGERDUMMY_H
