#ifndef PLATFORM_H
#define PLATFORM_H

#include "Base.h"
#include "Device.h"

#include <string>
#include <vector>

namespace CL {

	/**
	 * describes one platform (CPU, GPU, ..)
	 * available to OpenCL on this system
	 */
	class Platform {

	private:

		/** OpenCL internal ID */
		const cl_platform_id id;

		/** all devices attached to this platform */
		std::vector<Device*> devices;

		std::string profile;
		std::string version;
		std::string name;
		std::string vendor;
		std::string extensions;

	public:

		/** ctor */
		Platform(const cl_platform_id id) : id(id) {
			fetchDetails(id);
			fetchDevices(id);
		}

		/** dtor */
		~Platform() {
			for(Device* dev : devices) {delete dev;}
		}

		/** get the platform's internal ID */
		cl_platform_id getID() const {
			return id;
		}

		/** cast to string */
		operator std::string() const {
			return "PLATFORM: '" + name + "' (" + vendor + "): " + version + " [" + profile + "]";
		}

		/** get a list of all availble devices within this platform */
		const std::vector<Device*>& getDevices() {
			return devices;
		}

	private:

		/** fetch all details for the platform identified by ID */
		void fetchDetails(const cl_platform_id id) {

			static constexpr int MAX = 10240;
			char buffer[MAX];

			check(clGetPlatformInfo(id, CL_PLATFORM_PROFILE, MAX, buffer, nullptr));
			this->profile = buffer;

			check(clGetPlatformInfo(id, CL_PLATFORM_VERSION, MAX, buffer, nullptr));
			this->version = buffer;

			check(clGetPlatformInfo(id, CL_PLATFORM_NAME, MAX, buffer, nullptr));
			this->name = buffer;

			check(clGetPlatformInfo(id, CL_PLATFORM_VENDOR, MAX, buffer, nullptr));
			this->vendor = buffer;

			check(clGetPlatformInfo(id, CL_PLATFORM_EXTENSIONS, MAX, buffer, nullptr));
			this->extensions = buffer;

		}

		void fetchDevices(const cl_platform_id id) {

			static constexpr int MAX = 100;
			cl_device_id ids[MAX];
			cl_uint numDevices = 0;

			// fetch all devices (gpu, cpu, ..) available to this platform
			cl_int res = clGetDeviceIDs(id, CL_DEVICE_TYPE_ALL, MAX, ids, &numDevices);
			if (res == CL_DEVICE_NOT_FOUND) {warn("no devices found for this platform!");}
			else {check(res);}

			// fetch details for each of the detected devices
			for (cl_uint i = 0; i < numDevices; ++i) {
				Device* dev = new Device(this, ids[i]);
				devices.push_back(dev);
			}

		}



	};

}

#endif // PLATFORM_H
