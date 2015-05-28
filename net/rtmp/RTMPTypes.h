#ifndef K_NET_RTMP_RTMPTYPES_H
#define K_NET_RTMP_RTMPTYPES_H

/**
 * enum containing all supported stream types.
 *
 * there are still many types missing!!
 *
 */
enum RTMPTypes {

	SET_CHUNK_SIZE = 0x01,

	ACK = 0x03,

	CONTROL = 0x04,

	WINDOW_ACK = 0x05,

	SET_PEER_BANDWIDTH = 0x06,

	AUDIO = 0x08,

	AMF3 = 0x11,

	AMF0 = 0x12,

};

#endif // K_NET_RTMP_RTMPTYPES_H
