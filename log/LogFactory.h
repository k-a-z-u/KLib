#ifndef K_LOGFACTORY_H
#define K_LOGFACTORY_H

#include "Logger.h"
#include "../Exception.h"

namespace K {

	/**
	 * helper class to provide a global logger instance
	 * that is configured once at startup
	 */
	class LogFactory {

	public:

		/** get the current logger */
		static Logger* get() {
			Logger* l = *getPtr();
			if (!l) {throw Exception("logger not yet set()");}
			return l;
		}

		/** set the new logger */
		static void set(Logger* logger) {
			*getPtr() = logger;
		}

	private:

		/** static reference */
		static Logger** getPtr() {
			static Logger* logger = nullptr;
			return &logger;
		}

	};

}

#endif // K_LOGFACTORY_H
