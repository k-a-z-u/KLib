#ifndef K_CL_PROGRAM_H
#define K_CL_PROGRAM_H

#include "Base.h"
#include "Context.h"
#include "KernelFactory.h"
#include "../../Exception.h"

#include <fstream>
#include <string>

namespace K {
	namespace CL {

		class Program {

			CLASS_NAME("Program");

		private:

			/** the context this program belongs to */
			Context* context;

			/** the internal handle for the program */
			cl_program program;

			/** the kernel factory */
			KernelFactory kernFac;

			/** build options */
			std::string options;

			/** status indicator */
			bool build = false;

		private:

			friend class ProgramFactory;

			/** ctor */
			Program(Context* context) : context(context), kernFac(this) {
				verboseMe("ctor");
			}

		public:

			/** dtor */
			~Program() {
				verboseMeID(program, "dtor");
				clReleaseProgram(program); program = 0;
			}

			/** no-copy */
			Program(const Program& o) = delete;

			/** no-assign */
			Program& operator = (const Program& o) = delete;

		public:

			/** the KernelFactory */
			KernelFactory& kernels() {
				if (!build) {throw Exception("call program->setSource() first!");}
				return kernFac;
			}

			/** get the program's internal handle */
			cl_program getHandle() const {
				return program;
			}

			/** get the program's context */
			Context* getContext() {
				return context;
			}


			/** add paths to include in the search for #include files */
			void addIncludePath(const std::string& path) {
				options += " -I " + getPathForInc(path);
			}

			/** add the given definition to the compiler */
			void addDefinition(const std::string& def) {
				options += " -D " + def;
			}

			/** add the given definition to the compiler */
			void addDefinition(const std::string& def, const std::string& val) {
				options += " -D " + def + "=" + "\"" + val + "\"";
			}

			/**
			 * set the source-code and build the program.
			 * use "options" to supply additional build-options like:
			 *	-I /path/to/include
			 *	-D name=def
			 *	-D def
			 */
			void setSource(const std::string& code, const std::string& options = "") {

				verboseMe("settings source-code");

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

				// construct the final options string: given + global options
				const std::string opts = options + this->options;

				// build the program
				cl_int res = clBuildProgram(program, numDevices, deviceIDs, opts.data(), nullptr, nullptr);
				dumpBuildStats(program, devices);
				check(res);

				// status OK
				build = true;

			}

			/**
			 * set the program's source-code by reading the given file and build the program
			 * the given file's folder is automatically added as include path
			 */
			void setSourceFromFile(const std::string& file) {

				verboseMe("loading source file");

				std::ifstream fs (file, std::ios::ate);		// open at the end of the file
				if (!fs.good()) {throw "file not found?";}

				const size_t size = fs.tellg();				// current position = filesize
				char* buf = new char[size];
				fs.seekg (0, std::ios::beg);				// back to the beginning
				fs.read(buf, size);
				size_t read = fs.gcount();
				if (read != size) {throw "error";}

				const std::string code(buf, size);
				delete[] buf;

				// determine the file's include directory
				const std::string inc = getFolderForFile(file);

				// construct options strng
				const std::string opts = ("-I " + getPathForInc(inc));

				// compile
				setSource(code, opts);

			}

		private:

			/** get the given file's path-name */
			std::string getFolderForFile(const std::string file) const {

				#ifdef _WIN32
					const char sep = '\\';
				#else
					const char sep ='/';
				#endif

				const size_t pos = file.find_last_of(sep);
				return (pos == std::string::npos) ? ("") : (file.substr(0, pos));

			}

			/** modify the path to be safe for including */
			inline std::string getPathForInc(const std::string& path) const {
				return '"' + path + '"';
			}

			/** console dump of the build result */
			void dumpBuildStats(cl_program program, const std::vector<Device*> devices) const {

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
