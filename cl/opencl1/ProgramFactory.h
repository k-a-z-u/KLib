#ifndef K_CV_PROGRAMFACTORY_H
#define K_CV_PROGRAMFACTORY_H

#include <vector>
#include <string>

namespace K {
	namespace CL {

		class Context;
		class Program;

		/**
		 * factory for programs that belong to a context
		 */
		class ProgramFactory {

		private:

			/** the context the BufferFactory belongs to */
			Context* ctx;

			/** all programs created by the factory */
			std::vector<Program*> programs;

		private:

			friend class Context;

			/** hidden ctor */
			ProgramFactory(Context* ctx);

		public:

			/** dtor */
			~ProgramFactory();


			/** create a new, empty program. all created programs will be free-ed on factory destruction */
			Program* create();

			/** create a new program by loading the given file. all created programs will be free-ed on factory destruction */
			Program* createFromFile(const std::string& file);

			/** create a new program by loading the given source-code. all created programs will be free-ed on factory destruction */
			Program* createFromSource(const std::string& source);

			/** manually destroy the given program. do NOT use the pointer hereafter */
			void destroy(Program* prg);

		};

	}
}

#endif // K_CV_PROGRAMFACTORY_H
