#ifndef K_STREAMS_IOEXCEPTION_H
#define K_STREAMS_IOEXCEPTION_H

#include <string>

namespace K {

	/**
	 * IOException
	 */
	class IOException : public std::exception {

	public:

		/** ctor */
		IOException(const std::string& msg) : msg(msg) {;}

		const char* what() const throw() override {
			return msg.c_str();
		}

	private:

		/** the error message */
		std::string msg;

	};

}
#endif // K_STREAMS_IOEXCEPTION_H
