#ifndef CL_SYSTEM_H
#define CL_SYSTEM_H

#include "Base.h"
#include "Platform.h"

#include <vector>

namespace CL {

	class System {

	private:

		/** all detected platforms */
		std::vector<Platform*> platforms;

	public:

		/** ctor */
		System() {
			detectPlatforms();
		}

		/** dtor */
		~System() {
			for (Platform* p : platforms) {delete p;}
		}

		/** get a list of all availble platforms */
		const std::vector<Platform*>& getPlatforms() {
			return platforms;
		}

	private:

		/** detect all available platforms (CPUs, GPUs, ..) on this computer */
		void detectPlatforms() {

			static constexpr int MAX = 100;
			cl_platform_id platforms[MAX];
			cl_uint numPlatforms = 0;

			// fetch the IDs for all available platforms
			check( clGetPlatformIDs(MAX, platforms, &numPlatforms));

			// fetch the details for each available platform
			for (cl_uint i = 0; i < numPlatforms; ++i) {
				Platform* p = new Platform(platforms[i]);
				this->platforms.push_back(p);
			}

		}



	public:

	};

}\

#endif // CL_SYSTEM_H
