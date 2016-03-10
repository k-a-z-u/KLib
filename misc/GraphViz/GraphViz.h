#ifndef GRAPHVIZ_H
#define GRAPHVIZ_H

#include "../../os/Process.h"

namespace K {

	class GraphViz {

	public:

		/** ctor */
		GraphViz(const std::string file) : debug(false) {
			proc = new K::Process("/usr/bin/dot", {"-T", "png", "-o", file.c_str()});
		}

		/** dtor */
		~GraphViz() {
			close();
		}

		template <typename T>
		friend GraphViz& operator << (GraphViz& gv, const T& cmd) {
			gv.buffer << cmd;
			return gv;
		}

		/** set whether to output all commands as debug to terminal */
		void setDebugOutput(const bool debug) {
			this->debug = debug;
		}

		/** cleanup */
		void close() {
			if (proc) {
				proc->close();
				proc->join();
				delete proc;
				proc = nullptr;
			}
		}

		/** send all buffered commands to gnuplot */
		void flush() {
			*proc << buffer.str();
			if (debug) {std::cout << buffer.str() << std::endl;}
			proc->flush();
			buffer.str("");

//			std::string str;
//			*proc >> str;
//			std::cout << str << std::endl;
		}

		/** get the current buffer content as string */
		std::string getBuffer() const {
			return buffer.str();
		}

	private:

		/** io stream to gnuplot */
		K::Process* proc;

		/** the buffer to use */
		std::stringstream buffer;

		/** perform debug output? */
		bool debug;

	};

}

#endif // GRAPHVIZ_H
