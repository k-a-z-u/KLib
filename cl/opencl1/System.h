#ifndef K_CL_SYSTEM_H
#define K_CL_SYSTEM_H

#include "Base.h"
#include "Platform.h"
#include "ContextFactory.h"

#include <vector>

namespace K {
	namespace CL {


		class System {

			CLASS_NAME("System");

		private:

			/** all detected platforms */
			std::vector<Platform*> platforms;

			/** factory to create new contexts */
			ContextFactory ctxFac;

		public:

			/** ctor */
			System() {
				verboseMe("ctor");
				detectPlatforms();
			}

			/** dtor */
			~System() {
				verboseMe("dtor");
				for (Platform* p : platforms) {delete p;}
			}

			/** no-copy */
			System(const System& o) = delete;

			/** no-assign */
			System& operator = (const System& o) = delete;

		public:

			/** get the context factory */
			ContextFactory& contexts() {return ctxFac;}

			/** get a list of all availble platforms */
			const std::vector<Platform*>& getPlatforms() const {
				return platforms;
			}

			/** get the idx-th available platform */
			Platform* getPlatform(const int idx) const {
				if (idx >= platforms.size()) {throw Exception("platform index out of bounds");}
				return platforms[idx];
			}


		private:

			/** detect all available platforms (CPUs, GPUs, ..) on this computer */
			void detectPlatforms() {

				verboseMe("detecting available platforms");

				static constexpr int MAX = 100;
				cl_platform_id platforms[MAX];
				cl_uint numPlatforms = 0;

				// fetch the IDs for all available platforms
				check( clGetPlatformIDs(MAX, platforms, &numPlatforms));
				verboseMe("found " << numPlatforms << " platforms");

				// fetch the details for each available platform
				for (cl_uint i = 0; i < numPlatforms; ++i) {
					Platform* p = new Platform(platforms[i]);
					this->platforms.push_back(p);
				}

			}



		public:

		};

	}
}

#endif // K_CL_SYSTEM_H
