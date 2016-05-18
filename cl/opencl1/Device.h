#ifndef K_CL_DEVICE_H
#define K_CL_DEVICE_H

#include "Base.h"

#include <string>

namespace K {
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

			/** the maximum problem-dimension supported by the device. usually: 3 */
			cl_int maxDim;

			/** number of work-groups supported by the device? */
			cl_long maxWorkGroupSize;

			/** number of work-items (per dimension) supported by the device? */
			cl_long maxWorkItemSizePerDim[3];

		public:

			/** ctor */
			Device(Platform* platform, const cl_device_id id) : platform(platform), id(id) {
				fetchDetails(id);
			}

			/** dtor */
			~Device() {
				clReleaseDevice(id);
			}

			/** get the device's internal ID */
			cl_device_id getID() const {
				return id;
			}

			/** cast to string */
			operator std::string() const {
				return "DEVICE: '" + name + "' (" + vendor + ") [" + std::to_string(maxDim) + "D, " + std::to_string(maxWorkGroupSize) + "]";
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

				// work-groups / work-items
				check ( clGetDeviceInfo(id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(maxDim), &maxDim, nullptr ));
				check ( clGetDeviceInfo(id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(maxWorkGroupSize), &maxWorkGroupSize, nullptr ));
				check ( clGetDeviceInfo(id, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(maxWorkItemSizePerDim), maxWorkItemSizePerDim, nullptr ));

			}

		};

	}
}

#endif // K_CL_DEVICE_H
