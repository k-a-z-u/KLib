#ifndef K_OS_PROCESSEXCEPTION_H
#define K_OS_PROCESSEXCEPTION_H

#include <exception>
#include <string>

namespace K {


	/** exception handling within processes */
	class ProcessException : std::exception {
	public:
		ProcessException(const std::string& str) : str(str) { ; }
		const char* what() const throw() { return str.c_str(); }
	private:
		std::string str;
	};

}


#endif // K_OS_PROCESSEXCEPTION_H