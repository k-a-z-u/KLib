#ifndef K_CL_CONSTANTS_H
#define K_CL_CONSTANTS_H

#include <CL/cl.h>
#include <string>

namespace K {
	namespace CL {

		/**
		 * supported memory flags.
		 * those can be combined! e.g.
		 * flags = READ_WRITE | ALLOC_HOST_PTR;
		 */
		enum MemFlags : cl_mem_flags {

			READ_WRITE = CL_MEM_READ_WRITE,
			WRITE_ONLY = CL_MEM_WRITE_ONLY,
			READ_ONLY = CL_MEM_READ_ONLY,

			USE_HOST_PTR = CL_MEM_USE_HOST_PTR,
			ALLOC_HOST_PTR = CL_MEM_ALLOC_HOST_PTR,
			COPY_HOST_PTR = CL_MEM_COPY_HOST_PTR,

		};

		/**
		 * memory object type description
		 */
		enum MemObject : cl_mem_object_type {

			BUFFER = CL_MEM_OBJECT_BUFFER,
			IMAGE1D = CL_MEM_OBJECT_IMAGE1D,
			IMAGE2D = CL_MEM_OBJECT_IMAGE2D,
			IMAGE3D = CL_MEM_OBJECT_IMAGE3D

		};

		/**
		 * supported image channel orders
		 */
		enum ChannelOrder {
			R = CL_R,
			A = CL_A,
			RG = CL_RG,
			RA = CL_RA,
			RGB = CL_RGB,
			RGBA = CL_RGBA,
			BGRA = CL_BGRA,
			ARGB = CL_ARGB,
			INTENSITY = CL_INTENSITY,
			LUMINANCE = CL_LUMINANCE,
			Rx = CL_Rx,
			RGx = CL_RGx,
			RGBx = CL_RGBx,
			DEPTH = CL_DEPTH,
			DEPTH_STENCIL = CL_DEPTH_STENCIL,

#ifdef CL_VERSION_2_0
			sRGB = CL_sRGB,
			sRGBx = CL_sRGBx,
			sRGBA = CL_sRGBA,
			sBGRA = CL_sBGRA,
			ABGR = CL_ABGR,
#endif
		};

		/** convert ChannelOrder to string */
		inline static std::string toString(const ChannelOrder order) {
			switch (order) {
				case R: return "CL_R";
				case A: return "CL_A";
				case RG: return "CL_RG";
				case RA: return "CL_RA";
				case RGB: return "CL_RGB";
				case RGBA: return "CL_RGBA";
				case BGRA: return "CL_BGRA";
				case ARGB: return "CL_ARGB";
				case INTENSITY: return "CL_INTENSITY";
				case LUMINANCE: return "CL_LUMINANCE";
				case Rx: return "CL_Rx";
				case RGx: return "CL_RGx";
				case RGBx: return "CL_RGBx";
				case DEPTH: return "CL_DEPTH";
				case DEPTH_STENCIL: return "CL_DEPTH_STENCIL";
#ifdef CL_VERSION_2_0
				case sRGB: return "CL_sRGB";
				case sRGBx: return "CL_sRGBx";
				case sRGBA: return "CL_sRGBA";
				case sBGRA: return "CL_sBGRA";
				case ABGR: return "CL_ABGR";
#endif
				default: throw "missing ChannelOrder format";
			}
		}

		/**
		 * supported image channel types
		 */
		enum ChannelType {

			SNORM_INT8 = CL_SNORM_INT8,
			SNORM_INT16 = CL_SNORM_INT16,
			UNORM_INT8 = CL_UNORM_INT8,
			UNORM_INT16 = CL_UNORM_INT16,
			UNORM_SHORT_565 = CL_UNORM_SHORT_565,
			UNORM_SHORT_555 = CL_UNORM_SHORT_555,
			UNORM_INT_101010 = CL_UNORM_INT_101010,
			SIGNED_INT8 = CL_SIGNED_INT8,
			SIGNED_INT16 = CL_SIGNED_INT16,
			SIGNED_INT32 = CL_SIGNED_INT32,
			UNSIGNED_INT8 = CL_UNSIGNED_INT8,
			UNSIGNED_INT16 = CL_UNSIGNED_INT16,
			UNSIGNED_INT32 = CL_UNSIGNED_INT32,
			HALF_FLOAT = CL_HALF_FLOAT,
			FLOAT = CL_FLOAT,
			UNORM_INT24 = CL_UNORM_INT24,
#ifdef CL_VERSION_2_0
			UNORM_INT_101010_2 = CL_UNORM_INT_101010_2,
#endif

		};

		inline std::string toString(const ChannelType type) {
			switch(type) {
				case SNORM_INT8: return "CL_SNORM_INT8";
				case SNORM_INT16: return "CL_SNORM_INT16";
				case UNORM_INT8: return "CL_UNORM_INT8";
				case UNORM_INT16: return "CL_UNORM_INT16";
				case UNORM_SHORT_565: return "CL_UNORM_SHORT_565";
				case UNORM_SHORT_555: return "CL_UNORM_SHORT_555";
				case UNORM_INT_101010: return "CL_UNORM_INT_101010";
				case SIGNED_INT8: return "CL_SIGNED_INT8";
				case SIGNED_INT16: return "CL_SIGNED_INT16";
				case SIGNED_INT32: return "CL_SIGNED_INT32";
				case UNSIGNED_INT8: return "CL_UNSIGNED_INT8";
				case UNSIGNED_INT16: return "CL_UNSIGNED_INT16";
				case UNSIGNED_INT32: return "CL_UNSIGNED_INT32";
				case HALF_FLOAT: return "CL_HALF_FLOAT";
				case FLOAT: return "CL_FLOAT";
				case UNORM_INT24: return "CL_UNORM_INT24";
#ifdef CL_VERSION_2_0
				case UNORM_INT_101010_2: return "CL_UNORM_INT_101010_2";
#endif
				default: throw "missing ChannelType format";
			}
		}



	}
}

#endif // K_CL_CONSTANTS_H
