#ifndef K_NET_RTMP_RTMPLISTENER_H
#define K_NET_RTMP_RTMPLISTENER_H

#include <cstdint>
#include "amf/AMF.h"

namespace K {

	/**
	 * interface to listen for events within an RTMP transmission
	 */
	class RTMPListener {

	public:

		/** dtor */
		virtual ~RTMPListener() {;}

		/** received data with typeID on the given streamID */
		virtual void onRTMPData(const int streamID, const int typeID, const uint8_t control, const uint8_t* data, const uint32_t len) = 0;

		/** detected a new stream with the given ID and the provided content-type */
		virtual void onRTMPNewStream(const int streamID, const int typeID) = 0;

		/** start of a stream with the given ID */
		virtual void onRTMPStreamStart(const int streamID) = 0;

		/** end of a stream with the given ID */
		virtual void onRTMPStreamEnd(const int streamID) = 0;

		/** received AMF data */
		virtual void onRTMPAMF(AMFResult res) = 0;

		/** called when the transmission was completely reset (e.g. due to errors) */
		virtual void onRTMPReset() = 0;

	};

}

#endif // RTMPLISTENER_H
