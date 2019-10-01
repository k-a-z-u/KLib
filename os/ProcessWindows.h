#ifndef K_OS_PROCESSWINDOWS_H
#define K_OS_PROCESSWINDOWS_H

#define NOMINMAX
#include <Windows.h>
#undef near
#undef far


#include <chrono>
#include <sstream>
#include <thread>
#include <vector>

#include "ProcessException.h"

// see: https://msdn.microsoft.com/en-us/library/windows/desktop/ms682499(v=vs.85).aspx

namespace K {

	/**
	* @brief start an external process and communicate
	* via stream operators
	*/
	class Process {
		
	private:
		PROCESS_INFORMATION piProcInfo;

		HANDLE hChildStdInRx; // read
		HANDLE hChildStdInTx; // write
		HANDLE hChildStdOutRx;
		HANDLE hChildStdOutTx;

		/** send data to the process */
		std::stringstream ss;

	public:

		/** open the process using the given command */
		Process(const std::string& cmd) : Process(cmd, {}) {

		}

		/** open the process using the given command and arguments */
		Process(const std::string& cmd, const std::vector<std::string> args) : piProcInfo() {

			// Set the bInheritHandle flag so pipe handles are inherited. 
			SECURITY_ATTRIBUTES saAttr;
			saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
			saAttr.bInheritHandle = TRUE;
			saAttr.lpSecurityDescriptor = NULL;

			// Create a pipe for the child process's STDOUT. 
			if (!CreatePipe(&hChildStdOutRx, &hChildStdOutTx, &saAttr, 0)) {
				ThrowWin32Exception("Failed to create pipe for stdout.");
			}

			// Ensure the read handle to the pipe for STDOUT is not inherited.
			if (!SetHandleInformation(hChildStdOutRx, HANDLE_FLAG_INHERIT, 0)) {
				ThrowWin32Exception("Failed to disable handle inheritance for stdout pipe.");
			}

			// Create a pipe for the child process's STDIN. 
			if (!CreatePipe(&hChildStdInRx, &hChildStdInTx, &saAttr, 0)) {
				ThrowWin32Exception("Failed to create Pipe for stdin.");
			}

			// Ensure the write handle to the pipe for STDIN is not inherited. 
			if (!SetHandleInformation(hChildStdInTx, HANDLE_FLAG_INHERIT, 0)) {
				ThrowWin32Exception("Failed to disable handle inheritance for stdin pipe.");
			}
			
			// Create the child process. 
			STARTUPINFOA siStartInfo = {};
			siStartInfo.cb = sizeof(STARTUPINFOA);
			//siStartInfo.hStdError = hChildStdOutTx;
			//siStartInfo.hStdOutput = hChildStdOutTx;
			siStartInfo.hStdInput = hChildStdInRx;
			siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

			// build command line
			std::stringstream argStr;
			for (const std::string& arg : args) {
				argStr << arg << " ";
			}

			LPSTR szCommandLine = _strdup(argStr.str().c_str());
			
			// Use ACSII to not to deal with std::string LPTSTR conversion
			BOOL bSuccess = CreateProcessA(
				cmd.c_str(),		  // binary
				szCommandLine,        // command line 
				NULL,                 // process security attributes 
				NULL,                 // primary thread security attributes 
				TRUE,                 // handles are inherited 
				0,                    // creation flags 
				NULL,                 // use parent's environment 
				NULL,                 // use parent's current directory 
				&siStartInfo,         // STARTUPINFO pointer 
				&piProcInfo);         // receives PROCESS_INFORMATION 

			

			if (!bSuccess) {
				ThrowWin32Exception("Failed to create gnuplot process.");
			}
			
			free(szCommandLine);
		}


		/** dtor */
		~Process() {
			// cleanup
			close();
			kill();
		}

		void ThrowWin32Exception(const std::string& message)
		{
			DWORD dw = GetLastError();

			std::stringstream what;
			LPSTR errorText = NULL;		

			FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				dw,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPSTR)&errorText,
				0, NULL);

			what << message << " " << errorText;

   			LocalFree(errorText);

			throw ProcessException(what.str());
		}

		/** send command to process */
		template <typename type> Process& operator << (type data) {
			ss << data;
			return *this;
		}

		/** TODO read one line */
		Process& operator >> (std::string& str) {
			//char buf[128];
			//const ssize_t len = read(pipeFromProcess[READ], buf, 128);
			//std::cout << "read:" << len << std::endl;
			//if (len == -1) {
			//	throw new ProcessException("error while writing reading from process");
			//}
			//std::string tmp = std::string(buf, len);
			//str += tmp;
			//std::cout << "#" << tmp << "#" << std::endl;
			return *this;
		}



		/** flush to process */
		void flush() {
			const std::string data = ss.str();
			DWORD dwRead = data.size();
			DWORD dwWritten;
			BOOL bSuccess = FALSE;

			if (!WriteFile(hChildStdInTx, data.c_str(), dwRead, &dwWritten, NULL)) {
				ThrowWin32Exception("Failed to write to stdin.");
			}

			ss.str("");
		}

		/** close the streams */
		void close() {

			CloseHandle(hChildStdInRx);
			CloseHandle(hChildStdInTx);
			CloseHandle(hChildStdOutRx);
			CloseHandle(hChildStdOutTx);

			hChildStdInRx = 0;
			hChildStdInTx = 0;
			hChildStdOutRx = 0;
			hChildStdOutTx = 0;

			std::this_thread::sleep_for(std::chrono::milliseconds(10)); // dunno why but some processes need a little time here to close correctly
		}

		/** kill the process */
		void kill() {
			if (piProcInfo.hProcess) {
				if (!TerminateProcess(piProcInfo.hProcess, 0)) {
					ThrowWin32Exception("Failed to terminate gnuplot process.");
				}

				join();

				CloseHandle(piProcInfo.hProcess);
				CloseHandle(piProcInfo.hThread);

				piProcInfo = {};
			}
		}

		/** wait for the process to terminate */
		void join() {
			WaitForSingleObject(piProcInfo.hProcess, INFINITE);
		}
	};

}


#endif // K_OS_PROCESSWINDOWS_H