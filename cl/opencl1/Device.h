#ifndef K_CL_DEVICE_H
#define K_CL_DEVICE_H

#include "Base.h"
#include "DeviceAttributes.h"

#include <unordered_map>
#include <string>
#include <vector>

namespace K {
	namespace CL {

		class Platform;





		class Device {

			CLASS_NAME("Device");

		private:

			/** the platform this device belongs to */
			Platform* platform;

			/** the OpenCL internal ID */
			cl_device_id id;



			/** all loaded attributes */
			std::vector<DeviceAttr*> attrs;


		public:

			/** ctor */
			Device(Platform* platform, const cl_device_id id) : platform(platform), id(id) {
				verboseMeID(id, "ctor");
				fetchDetails(id);
			}

			/** dtor */
			~Device() {
				verboseMeID(id, "dtor");
				clReleaseDevice(id);
				for (DeviceAttr* attr : attrs) {delete attr;}
			}

			/** no-copy */
			Device(const Device& o) = delete;

			/** no-assign */
			Device& operator = (const Device& o) = delete;


			/** get the device's internal ID */
			cl_device_id getID() const {
				return id;
			}

			/** cast to string */
			operator std::string() const {
				return	"DEVICE: '" + getAttr(DeviceAttribute::NAME)->toString() +
						"' (" + getAttr(DeviceAttribute::VENDOR)->toString() + ")";
			}

			/** get the platform this device belongs to */
			Platform* getPlatform() const {
				return platform;
			}

			/** get a DeviceAttribute by its enum index */
			const DeviceAttr* getAttr(const DeviceAttribute attr) const {
				return attrs[attr];
			}

			/** dump all device attributes */
			void dumpAttributes() {
				std::cout << (std::string) *this << std::endl;
				for (const DeviceAttr* attr : attrs) {
					std::cout << "\t" << attr->name << ": " << attr->toString() << std::endl;
				}
			}


		private:

			/** feth some details for this device */
			void fetchDetails(const cl_device_id id) {

				verboseMeID(id, "fetching details");

				attrs.resize(DeviceAttribute::_END);

				// setup
				attrs[DeviceAttribute::NAME] =							new DeviceAttrString("name", CL_DEVICE_NAME);
				attrs[DeviceAttribute::VENDOR] =						new DeviceAttrString("vendor", CL_DEVICE_VENDOR);
				attrs[DeviceAttribute::DEVICE_VERSION] =				new DeviceAttrString("device version", CL_DEVICE_VERSION);
				attrs[DeviceAttribute::DEVICE_PROFILE] =				new DeviceAttrString("device profile", CL_DEVICE_PROFILE);
				attrs[DeviceAttribute::DRIVER_VERSION] =				new DeviceAttrString("driver version", CL_DRIVER_VERSION);

				attrs[DeviceAttribute::MAX_DIMENSION] =					new DeviceAttrInt("max dimension", CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS);
				attrs[DeviceAttribute::MAX_WORK_GROUP_SIZE] =			new DeviceAttrLong("max WorkGroup size", CL_DEVICE_MAX_WORK_GROUP_SIZE);
				attrs[DeviceAttribute::MAX_WORK_ITEM_SIZE_PER_DIM] =	new DeviceAttrLongArr("max WorkItem size per dimension", CL_DEVICE_MAX_WORK_ITEM_SIZES, 3);
				attrs[DeviceAttribute::COMPILER_AVAILABLE] =			new DeviceAttrBool("compiler available", CL_DEVICE_COMPILER_AVAILABLE);
				attrs[DeviceAttribute::HAS_ECC] =						new DeviceAttrBool("has ECC", CL_DEVICE_ERROR_CORRECTION_SUPPORT);
				attrs[DeviceAttribute::MAX_MEM_ALLOC_SIZE] =			new DeviceAttrLong("max mem alloc size", CL_DEVICE_MAX_MEM_ALLOC_SIZE);
				attrs[DeviceAttribute::GLOBAL_MEM_CACHE_SIZE] =			new DeviceAttrLong("global mem cache size", CL_DEVICE_GLOBAL_MEM_CACHE_SIZE);
				attrs[DeviceAttribute::GLOBAL_MEM_SIZE] =				new DeviceAttrLong("global mem size", CL_DEVICE_GLOBAL_MEM_SIZE);
				attrs[DeviceAttribute::LOCAL_MEM_SIZE] =				new DeviceAttrLong("local mem size", CL_DEVICE_LOCAL_MEM_SIZE);
				attrs[DeviceAttribute::HAS_IMAGE_SUPPORT] =				new DeviceAttrBool("image support", CL_DEVICE_IMAGE_SUPPORT);
				attrs[DeviceAttribute::IMAGE2D_MAX_WIDTH] =				new DeviceAttrSizeT("max 2D image width", CL_DEVICE_IMAGE2D_MAX_WIDTH);
				attrs[DeviceAttribute::IMAGE2D_MAX_HEIGHT] =			new DeviceAttrSizeT("max 2D image height", CL_DEVICE_IMAGE2D_MAX_HEIGHT);
				attrs[DeviceAttribute::IMAGE3D_MAX_WIDTH] =				new DeviceAttrSizeT("max 3D image width", CL_DEVICE_IMAGE3D_MAX_WIDTH);
				attrs[DeviceAttribute::IMAGE3D_MAX_HEIGHT] =			new DeviceAttrSizeT("max 3D image height", CL_DEVICE_IMAGE3D_MAX_HEIGHT);
				attrs[DeviceAttribute::IMAGE3D_MAX_DEPTH] =				new DeviceAttrSizeT("max 3D image depth", CL_DEVICE_IMAGE3D_MAX_DEPTH);
				attrs[DeviceAttribute::MAX_COMPUTE_UNITS] =				new DeviceAttrInt("max compue units", CL_DEVICE_MAX_COMPUTE_UNITS);
				attrs[DeviceAttribute::MAX_SAMPLERS_PER_KERNEL] =		new DeviceAttrInt("max samplers/kernel", CL_DEVICE_MAX_SAMPLERS);
				attrs[DeviceAttribute::DEVICE_MAX_CLOCK_FREQ] =			new DeviceAttrInt("max clock frequency", CL_DEVICE_MAX_CLOCK_FREQUENCY);

				// load
				for (DeviceAttr* attr : attrs) {
					if (attr != nullptr) {attr->fetch(id);}
				}


			}

		};

	}
}

#endif // K_CL_DEVICE_H
