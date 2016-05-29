#ifndef K_CV_PROGRAMFACTORYIMPL_H
#define K_CV_PROGRAMFACTORYIMPL_H

#include "Context.h"
#include "Program.h"
#include "ProgramFactory.h"

namespace K {
	namespace CL {


		ProgramFactory::ProgramFactory(Context* ctx) : ctx(ctx) {
			;
		}

		ProgramFactory::~ProgramFactory() {
			for (Program* prog : programs) {delete prog;}
		}

		Program* ProgramFactory::create() {
			Program* prog = new Program(ctx);
			programs.push_back(prog);
			return prog;
		}

		Program* ProgramFactory::createFromFile(const std::string& file)  {
			Program* prog = new Program(ctx);
			programs.push_back(prog);
			prog->setSourceFromFile(file);
			return prog;
		}

		Program* ProgramFactory::createFromSource(const std::string& source)  {
			Program* prog = new Program(ctx);
			programs.push_back(prog);
			prog->setSource(source);
			return prog;
		}

	}
}

#endif // K_CV_PROGRAMFACTORYIMPL_H
