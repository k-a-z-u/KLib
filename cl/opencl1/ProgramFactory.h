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


			/** create a new, empty program */
			Program* create();

			/** create a new program by loading the given file */
			Program* createFromFile(const std::string& file);

			/** create a new program by loading the given source-code */
			Program* createFromSource(const std::string& source);


		};

	}
}

#endif // K_CV_PROGRAMFACTORY_H
