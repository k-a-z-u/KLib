#ifndef DEVICE_H
#define DEVICE_H

#include "Base.h"
#include <string>

namespace CL {

	class Platform;

	class Device {

	private:

		/** the platform this device belongs to */
		Platform* platform;

		/** the OpenCL internal ID */
		cl_device_id id;

		std::string name;
		std::string vendor;

	public:

		/** ctor */
		Device(Platform* platform, const cl_device_id id) : platform(platform), id(id) {
			fetchDetails(id);
		}

		/** get the device's internal ID */
		cl_device_id getID() const {
			return id;
		}

		/** cast to string */
		operator std::string() const {
			return "DEVICE: '" + name + "' (" + vendor + ")";
		}

		/** get the platform this device belongs to */
		Platform* getPlatform() const {
			return platform;
		}

	private:

		/** feth some details for this device */
		void fetchDetails(const cl_device_id id) {

			static constexpr int MAX = 10240;
			char buffer[MAX];

			check( clGetDeviceInfo(id, CL_DEVICE_NAME, MAX, buffer, nullptr));
			this->name = buffer;

			check( clGetDeviceInfo(id, CL_DEVICE_VENDOR, MAX, buffer, nullptr));
			this->vendor = buffer;

		}

	};

}

#endif // DEVICE_H
