#ifndef K_LOG_LOGGER_H
#define K_LOG_LOGGER_H

#include <string>
#include "LogLevel.h"
#include "../os/Time.h"

namespace K {

	class Logger {

	protected:

		/** the log-level to use (discard some entries) */
		LogLevel lvl;

	public:

		/** ctor */
		Logger() : lvl(LogLevel::DEBUG) {
			;
		}

		virtual ~Logger() {;}

		/** add a new message to the log */
		virtual void add(const std::string& component, const LogLevel lvl, const std::string& msg) {
			if ((int) lvl < (int) this->lvl) {return;}
			const uint64_t ts = Time::getTimeMS();
			add(ts, component, lvl, msg);
		}

		/** set the log-level to use */
		void setLogLevel(const LogLevel lvl) {
			this->lvl = lvl;
		}

	protected:

		virtual void add(const uint64_t ts, const std::string& component, const LogLevel lvl, const std::string& msg) = 0;

		/** convert from level-enum to readable string */
		static std::string levelToString(const LogLevel lvl) {
			switch (lvl) {
				case LogLevel::DEBUG:	return " DBG";
				case LogLevel::INFO:	return "INFO";
				case LogLevel::WARNING:	return "WARN";
				case LogLevel::ERROR:	return " ERR";
			}
			throw "unsupported level given";
		}

	};

}

#endif // LOGGER_H
