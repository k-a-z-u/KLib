#ifndef K_CL_PLATFORM_H
#define K_CL_PLATFORM_H

#include "Base.h"
#include "Device.h"

#include <unordered_map>
#include <string>
#include <vector>

namespace K {
	namespace CL {

		class System;

		/** describes one platform attribute */
		struct PlatformAttr {
			std::string name;
			PlatformAttr(const std::string& name) : name(name) {;}
		};

		/** supported platform attributes */
		namespace PlatformAttrs {
			static const PlatformAttr PROFILE = PlatformAttr("profile");
			static const PlatformAttr VERSION = PlatformAttr("version");
			static const PlatformAttr NAME = PlatformAttr("name");
			static const PlatformAttr VENDOR = PlatformAttr("vendor");
			static const PlatformAttr EXTENSIONS = PlatformAttr("extensions");
		}

		/**
		 * describes one platform (CPU, GPU, ..)
		 * available to OpenCL on this system
		 */
		class Platform {

			CLASS_NAME("Platform");

		private:

			/** OpenCL internal ID */
			const cl_platform_id id;

			/** all devices attached to this platform */
			std::vector<Device*> devices;

			/** all platform attributes */
			std::unordered_map<const PlatformAttr*, std::string> attrs;

		private:

			friend class System;

			/** hidden ctor. use System.getPlatforms() */
			Platform(const cl_platform_id id) : id(id) {
				verboseMeID(id, "ctor");
				fetchDetails(id);
				fetchDevices(id);
			}

		public:

			/** dtor */
			~Platform() {
				verboseMeID(id, "dtor");
				// cl_platform_id does not need to be released
				for(Device* dev : devices) {delete dev;}
			}

			/** no-copy */
			Platform(const Platform& o) = delete;

			/** no-assign */
			Platform& operator = (const Platform& o) = delete;

		public:

			/** get the platform's internal handle */
			cl_platform_id getHandle() const {
				return id;
			}

			/** get the value for the given attribute */
			std::string getAttr(const PlatformAttr& attr) const {
				auto it = attrs.find(&attr);
				return (it == attrs.end()) ? ("?") : (it->second);
			}

			/** cast to string */
			operator std::string() const {
				return	"PLATFORM: '" + getAttr(PlatformAttrs::NAME) +
						"' (" + getAttr(PlatformAttrs::VENDOR) + "): " +
						getAttr(PlatformAttrs::VERSION) +
						" [" + getAttr(PlatformAttrs::PROFILE) + "]";
			}

			/** get a list of all availble devices within this platform */
			const std::vector<Device*>& getDevices() const {
				return devices;
			}

			/** get the idx-th device within this platform */
			Device* getDevice(const int idx) const {
				return devices[idx];
			}

		private:

			/** fetch all details for the platform identified by ID */
			void fetchDetails(const cl_platform_id id) {

				verboseMeID(id, "fetching details");

				static constexpr int MAX = 10240;
				char buffer[MAX];

				check(clGetPlatformInfo(id, CL_PLATFORM_PROFILE, MAX, buffer, nullptr));
				attrs[&PlatformAttrs::PROFILE] = buffer;

				check(clGetPlatformInfo(id, CL_PLATFORM_VERSION, MAX, buffer, nullptr));
				attrs[&PlatformAttrs::VERSION] = buffer;

				check(clGetPlatformInfo(id, CL_PLATFORM_NAME, MAX, buffer, nullptr));
				attrs[&PlatformAttrs::NAME] = buffer;

				check(clGetPlatformInfo(id, CL_PLATFORM_VENDOR, MAX, buffer, nullptr));
				attrs[&PlatformAttrs::VENDOR] = buffer;

				check(clGetPlatformInfo(id, CL_PLATFORM_EXTENSIONS, MAX, buffer, nullptr));
				attrs[&PlatformAttrs::EXTENSIONS] = buffer;

			}

			/** fetch a list of all devices that belong to this platform */
			void fetchDevices(const cl_platform_id id) {

				verboseMeID(id, "fetching devices");

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
}

#endif // K_CL_PLATFORM_H
