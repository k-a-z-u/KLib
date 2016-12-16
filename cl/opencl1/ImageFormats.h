#ifndef K_CL_IMAGEFORMATS_H
#define K_CL_IMAGEFORMATS_H

#include "Constants.h"

namespace K {
	namespace CL {

		/** denotes one supported combination of channel-type and channel-order */
		struct SupportedImageFormat {

			ChannelOrder order;
			ChannelType type;

			/** ctor */
			SupportedImageFormat(const ChannelOrder order, const ChannelType type) : order(order), type(type) {;}

			/** check for equality */
			bool operator == (const SupportedImageFormat& o) const {
				return (o.type == type) && (o.order == order);
			}

			/** readable string */
			std::string asString() const {
				return toString(order) + " - " + toString(type);
			}

		};

		/** vector containing several ImageFormats */
		using SupportedImageFormats = std::vector<SupportedImageFormat>;

	}

}

#endif // K_CL_IMAGEFORMATS_H
