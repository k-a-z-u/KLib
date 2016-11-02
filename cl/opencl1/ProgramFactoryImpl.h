#ifndef K_CV_PROGRAMFACTORYIMPL_H
#define K_CV_PROGRAMFACTORYIMPL_H

#include "Context.h"
#include "Program.h"
#include "ProgramFactory.h"

#include <algorithm>

namespace K {
	namespace CL {

		inline ProgramFactory::ProgramFactory(Context* ctx) : ctx(ctx) {
			;
		}

		inline ProgramFactory::~ProgramFactory() {
			for (Program* prog : programs) {delete prog;}
		}

		inline Program* ProgramFactory::create() {
			Program* prog = new Program(ctx);
			programs.push_back(prog);
			return prog;
		}

		inline Program* ProgramFactory::createFromFile(const std::string& file)  {
			Program* prog = new Program(ctx);
			programs.push_back(prog);
			prog->setSourceFromFile(file);
			return prog;
		}

		inline Program* ProgramFactory::createFromSource(const std::string& source)  {
			Program* prog = new Program(ctx);
			programs.push_back(prog);
			prog->setSource(source);
			return prog;
		}

		inline void ProgramFactory::destroy(Program* prg) {
			programs.erase(std::find(programs.begin(), programs.end(), prg));
			delete prg;
		}

	}
}

#endif // K_CV_PROGRAMFACTORYIMPL_H
