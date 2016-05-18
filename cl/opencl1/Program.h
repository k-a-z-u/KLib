#ifndef PROGRAM_H
#define PROGRAM_H

#include "Base.h"
#include "Context.h"

#include <fstream>
#include <string>

namespace CL {

	class Program {

	private:

		/** the context this program belongs to */
		Context* context;

		/** the internal handle for the program */
		cl_program program;

	public:

		/** ctor */
		Program(Context* context) : context(context) {
			;
		}

		/** get the program's internal handle */
		cl_program getID() const {
			return program;
		}

		/** get the program's context */
		Context* getContext() {
			return context;
		}

		/** set the setSourceprogram's source-code */
		void setSource(const std::string& code) {

			const char* codeArr[1] = {code.data()};
			const size_t sizeArr[1] = {code.length()};
			cl_int status = 0;

			// setup the program
			program = clCreateProgramWithSource(context->getID(), 1, codeArr, sizeArr, &status);
			check(status);

			// setup the array of all devices that belong to the context
			const int numDevices = context->getAttachedDevices().size();
			cl_device_id deviceIDs[100];
			for (size_t i = 0; i < context->getAttachedDevices().size(); ++i) {
				deviceIDs[i] = context->getAttachedDevices()[i]->getID();
			}

			// build the program
			const std::string options;
			cl_int res = clBuildProgram(program, numDevices, deviceIDs, options.data(), nullptr, nullptr);
			dumpBuildStats(program, context->getAttachedDevices());
			check(res);

		}

		/** set the program's source-code be reading the given file */
		void setSourceFromFile(const std::string& file) {

			std::ifstream fs (file, std::ios::ate);		// open at the end of the file
			if (!fs.good()) {throw "file not found?";}

			const size_t size = fs.tellg();				// current position = filesize
			char* buf = new char[size];
			fs.seekg (0, std::ios::beg);				// back to the beginning
			fs.read(buf, size);
			size_t read = fs.gcount();
			if (read != size) {throw "error";}

			const std::string code(buf, size);
			delete buf;

			setSource(code);

		}

	private:

		void dumpBuildStats(cl_program program, const std::vector<Device*> devices) {

			char buffer[64*1024];
			size_t used = 0;

			// dump the program build status for every attached device
			for (Device* dev : devices) {
				std::cout << "BUILD_LOG FOR " << (std::string) *dev << std::endl;
				cl_device_id devID = dev->getID();
				check( clGetProgramBuildInfo(program, devID, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &used) );
				std::cout << std::string(buffer) << std::endl;
				std::cout << std::endl;
			}

		}


	};

}
#endif // PROGRAM_H
