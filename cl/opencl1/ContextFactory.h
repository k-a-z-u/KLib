#ifndef K_CV_CONTEXTFACTORY_H
#define K_CV_CONTEXTFACTORY_H

#include <vector>
#include "Context.h"

namespace K {
	namespace CL {

		class ContextFactory {

		private:

			/** all created contexts */
			std::vector<Context*> contexts;

		public:

			/** dtor */
			~ContextFactory() {
				for (Context* ctx : contexts) {delete ctx;}
			}

			/** create a new context */
			Context* create() {
				Context* ctx = new Context();
				contexts.push_back(ctx);
				return ctx;
			}

		};

	}
}

#endif // K_CV_CONTEXTFACTORY_H
