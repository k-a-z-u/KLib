#ifndef K_CL_PROGRAM_H
#define K_CL_PROGRAM_H

#include "Base.h"
#include "Context.h"

#include <fstream>
#include <string>

namespace K {
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

			/** dtor */
			~Program() {
				clReleaseProgram(program); program = 0;
			}

			/** no-copy */
			Program(const Program& o) = delete;

			/** no-assign */
			Program& operator = (const Program& o) = delete;

		public:

			/** get the program's internal handle */
			cl_program getHandle() const {
				return program;
			}

			/** get the program's context */
			Context* getContext() {
				return context;
			}

			/** set the source-code and build the program */
			void setSource(const std::string& code) {

				const char* codeArr[1] = {code.data()};
				const size_t sizeArr[1] = {code.length()};
				cl_int status = 0;

				// setup the program
				program = clCreateProgramWithSource(context->getHandle(), 1, codeArr, sizeArr, &status);
				check(status);

				// all devices attached to the context
				std::vector<Device*> devices = context->getAttachedDevices();

				// setup the array of all devices that belong to the context
				const cl_uint numDevices = (cl_uint) devices.size();
				cl_device_id deviceIDs[100];
				for (size_t i = 0; i < devices.size(); ++i) {
					deviceIDs[i] = devices[i]->getID();
				}

				// build the program
				const std::string options;
				cl_int res = clBuildProgram(program, numDevices, deviceIDs, options.data(), nullptr, nullptr);
				dumpBuildStats(program, devices);
				check(res);

			}

			/** set the program's source-code by reading the given file and build the program */
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

			/** console dump of the build result */
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
}

#endif // K_CL_PROGRAM_H
