#ifndef K_CV_DEVICEATTRIBUTES_H
#define K_CV_DEVICEATTRIBUTES_H

#include "Base.h"
#include <string>

namespace K {
	namespace CL {

		/** all supported attributes */
		enum DeviceAttribute {
			NAME,
			VENDOR,
			DEVICE_VERSION,
			DEVICE_PROFILE,
			DRIVER_VERSION,
			MAX_DIMENSION,
			MAX_WORK_GROUP_SIZE,
			MAX_WORK_ITEM_SIZE_PER_DIM,
			COMPILER_AVAILABLE,
			HAS_ECC,
			GLOBAL_MEM_CACHE_SIZE,
			GLOBAL_MEM_SIZE,
			LOCAL_MEM_SIZE,
			HAS_IMAGE_SUPPORT,
			IMAGE2D_MAX_WIDTH,
			IMAGE2D_MAX_HEIGHT,
			MAX_COMPUTE_UNITS,
			MAX_SAMPLERS_PER_KERNEL,
			DEVICE_MAX_CLOCK_FREQ,
			_END,
		};

		/** describes one device attribute */
		struct DeviceAttr {
			std::string name;
			cl_device_info param;
			DeviceAttr(const std::string& name, const cl_device_info param) : name(name), param(param) {;}
			~DeviceAttr() {;}
			virtual std::string toString() const = 0;
			virtual void fetch(cl_device_id id) = 0;
		};



		struct DeviceAttrBool : public DeviceAttr {
			cl_bool val;
			DeviceAttrBool(const std::string& name, const cl_device_info param) : DeviceAttr(name, param) {;}
			std::string toString() const {return (val) ? ("true") : ("false");}
			void fetch(cl_device_id id) {check( clGetDeviceInfo(id, param, sizeof(cl_bool), &val, nullptr));}
		};

		struct DeviceAttrString : public DeviceAttr {
			std::string val;
			DeviceAttrString(const std::string& name, const cl_device_info param) : DeviceAttr(name, param) {;}
			std::string toString() const {return val;}
			void fetch(cl_device_id id) {
				const char buf[4096] = {};
				check( clGetDeviceInfo(id, param, 4096, (void*) buf, nullptr));
				val = buf;
			}
		};

		struct DeviceAttrInt : public DeviceAttr {
			cl_int val;
			DeviceAttrInt(const std::string& name, const cl_device_info param) : DeviceAttr(name, param) {;}
			std::string toString() const {return std::to_string(val);}
			void fetch(cl_device_id id) {check( clGetDeviceInfo(id, param, sizeof(cl_int), &val, nullptr));}
		};

		struct DeviceAttrLong : public DeviceAttr {
			cl_long val;
			DeviceAttrLong(const std::string& name, const cl_device_info param) : DeviceAttr(name, param) {;}
			std::string toString() const {return std::to_string(val);}
			void fetch(cl_device_id id) {check( clGetDeviceInfo(id, param, sizeof(cl_long), &val, nullptr));}
		};

		struct DeviceAttrSizeT : public DeviceAttr {
			size_t val;
			DeviceAttrSizeT(const std::string& name, const cl_device_info param) : DeviceAttr(name, param) {;}
			std::string toString() const {return std::to_string(val);}
			void fetch(cl_device_id id) {check( clGetDeviceInfo(id, param, sizeof(size_t), &val, nullptr));}
		};

		struct DeviceAttrLongArr : public DeviceAttr {
			cl_long val[16];
			int cnt;
			DeviceAttrLongArr(const std::string& name, const cl_device_info param, const int cnt) : DeviceAttr(name, param), cnt(cnt) {;}
			void fetch(cl_device_id id) {check( clGetDeviceInfo(id, param, sizeof(cl_long)*cnt, &val, nullptr));}
			std::string toString() const {
				std::string s;
				for (int i = 0; i < cnt; ++i) {s += std::to_string(val[i]) + ", ";}
				return s;
			}
		};

	}
}
#endif // K_CV_DEVICEATTRIBUTES_H
