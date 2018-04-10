#ifndef K_OS_PROCESSLINUX_H
#define K_OS_PROCESSLINUX_H

#include <exception>
#include <iosfwd>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#include <fcntl.h>
#include <sstream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "ProcessException.h"

namespace K {

	/**
	* @brief start an external process and communicate
	* via stream operators
	*/
	class Process {

		const int CHILD = 0;
		const int READ = 0;
		const int WRITE = 1;
		//const int ERR = 2;

	public:

		/** open the process using the given command */
		Process(const std::string& cmd) : Process(cmd, {}) {

		}

		/** open the process using the given command and arguments */
		Process(const std::string& cmd, const std::vector<std::string> args) : pid(0), pipeToProcess(), pipeFromProcess() {

			// create pipes
			if (pipe(pipeToProcess) != 0) {
				throw ProcessException("error while creating pipe for process: " + cmd);
			}
			if (pipe(pipeFromProcess) != 0) {
				throw ProcessException("error while creating pipe for process: " + cmd);
			}

			// argument pointer
			char* argv[128];					// binary + args + null-terminator
												//char* argv[args.size()+2];			// binary + args + null-terminator
			argv[0] = (char*)cmd.c_str();		// the binary itself
			argv[args.size() + 1] = nullptr;		// null-terminator
			for (int i = 0; i < (int)args.size(); ++i) {
				argv[i + 1] = (char*)args[i].c_str();
			}

			// fork a new process
			pid = fork();

			// check how to proceed
			if (pid < 0) {

				// fork failed
				throw ProcessException("error while creating fork for process: " + cmd);

			}
			else if (pid == CHILD) {

				// child process

				// close pipes not needed by the child
				closeFD(pipeToProcess[WRITE]);
				closeFD(pipeFromProcess[READ]);
				//close(pipeFromProcess[ERR]);

				// reading from what parent has written to this process
				dup2(pipeToProcess[READ], STDIN_FILENO);

				// writing this process's output to the parent
				dup2(pipeFromProcess[WRITE], STDOUT_FILENO);
				//dup2(pipeFromProcess[ERR], STDERR_FILENO);

				// exec
				const int ret = execv(cmd.c_str(), argv);		// execl

																// check
				if (ret < 0) {
					throw ProcessException("error while executing process: " + cmd);
					exit(0);
				}


			}
			else {

				// parent process

				// close pipes not needed by the parent
				closeFD(pipeToProcess[READ]);
				closeFD(pipeFromProcess[WRITE]);
				//close(pipeFromProcess[ERR]);

			}

		}


		/** dtor */
		~Process() {

			// cleanup
			close();
			kill();

		}

		/** send command to process */
		template <typename type> Process& operator << (type data) {
			ss << data;
			return *this;
		}

		/** TODO read one line */
		Process& operator >> (std::string& str) {
			char buf[128];
			const ssize_t len = read(pipeFromProcess[READ], buf, 128);
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
			const std::string data = ss.str();
			ssize_t start = 0;
			ssize_t todo = data.length();
			while (todo > 0) {
				const ssize_t ret = write(pipeToProcess[WRITE], data.c_str() + start, data.length() - start);
				if (ret == -1) { throw new ProcessException("error while writing to process"); }
				todo -= ret;
				start += ret;
			}
			//			if (ret != (long) data.length()) {
			//				throw new ProcessException("error writing complete data block to child. FIXME.");
			//			}
			ss.str("");

		}

		/** close the streams */
		void close() {
			closeFD(pipeFromProcess[READ]);
			closeFD(pipeToProcess[WRITE]);
			usleep(1000 * 10);	// dunno why but some processes need a little time here to close correctly
		}

		/** kill the process */
		void kill() {
			if (pid) {
				const int res = ::kill(pid, SIGTERM);
				if (res != 0) {
					::kill(pid, SIGKILL);
				}
				join();
				pid = 0;
			}
		}

		/** wait for the process to terminate */
		void join() {
			int status;
			while (wait(&status) != pid) {
				usleep(1000 * 10);
			}
		}

	private:

		/** cleanup */
		void closeFD(int& fd) {
			if (fd >= 0) { ::close(fd); fd = -1; }
		}

		/** send data to the process */
		std::stringstream ss;

		/** the child's process id */
		pid_t pid;

		/** pipe to send data to the process */
		int pipeToProcess[3];

		/** pipe to read data from the process */
		int pipeFromProcess[3];

	};

}

#endif // K_OS_PROCESSLINUX_H