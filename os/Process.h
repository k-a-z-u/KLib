#ifndef K_OS_PROCESS_H
#define K_OS_PROCESS_H

#include <iosfwd>
#include <cstdio>
#include <exception>
#include <fcntl.h>
#include <sstream>

namespace K {


	/** exception handling within processes */
	class ProcessException : std::exception {
	public:
		ProcessException(const std::string& str) : str(str) {;}
		const char* what() const throw() {return str.c_str();}
	private:
		std::string str;
	};


	/**
	 * @brief start an external process and communicate
	 * via stream operators
	 */
	class Process {

		const int CHILD = 0;
		const int READ = 0;
		const int WRITE = 1;

	public:

		/** open the process using the given command */
		Process(const std::string& cmd) : pid(0), pipeToProcess(), pipeFromProcess() {

			// create pipes
			if ( pipe(pipeToProcess) != 0 ) {
				throw ProcessException("error while creating pipe for process: " + cmd);
			}
			if ( pipe(pipeFromProcess) != 0 ) {
				throw ProcessException("error while creating pipe for process: " + cmd);
			}

			// fork a new process
			pid = fork();

			// check how to proceed
			if ( pid < 0) {

				// fork failed
				throw ProcessException("error while creating fork for process: " + cmd);

			} else if ( pid == CHILD ) {

				// child process

				// close pipes not needed by the child
				close(pipeToProcess[WRITE]);
				close(pipeFromProcess[READ]);

				// reading from what parent has written to this process
				dup2(pipeToProcess[READ], STDIN_FILENO);

				// writing this process's output to the parent
				dup2(pipeFromProcess[WRITE], STDOUT_FILENO);
				//dup2(pipeFromProcessErr[WRITE], STDERR_FILENO);

				// exec
				int ret = execl(cmd.c_str(), "", (char*) NULL);

				// check
				if (ret < 0) {
					throw ProcessException("error while executing process: " + cmd);
				}
				exit(0);

			} else {

				// parent process

				// close pipes not needed by the parent
				close(pipeToProcess[READ]);
				close(pipeFromProcess[WRITE]);

			}

		}

		/** dtor */
		~Process() {

			// cleanup
			close(pipeToProcess[READ]);
			close(pipeFromProcess[WRITE]);
			kill(pid, 9);

		}

		/** send command to process */
		template <typename type> Process& operator << (type data) {
			ss << data;
			return *this;
		}

		/** TODO read one line */
		Process& operator >> (std::string& str) {
			char buf[128];
			ssize_t len = read( pipeFromProcess[READ], buf, 128 );
			std::cout << "read:" << len << std::endl;
			if (len == -1) {
				throw new ProcessException("error while writing reading from process");
			}
			std::string tmp = std::string(buf, len);
			str += tmp;
			std::cout << "#" << tmp << "#" << std::endl;
			return *this;
		}



		/** flush to process */
		void flush() {
			std::string data = ss.str();
			ssize_t ret = write( pipeToProcess[WRITE], data.c_str(), data.length() );
			if (ret != (long) data.length()) {
				throw new ProcessException("error writing complete data block to child. FIXME.");
			}
			ss.str("");
			if (ret == -1) {
				throw new ProcessException("error while writing to process");
			}
		}




		/** send data to the process */
		std::stringstream ss;

		/** the child's process id */
		pid_t pid;

		/** pipe to send data to the process */
		int pipeToProcess[2];

		/** pipe to read data from the process */
		int pipeFromProcess[2];

	};

}

#endif // K_OS_PROCESS_H
