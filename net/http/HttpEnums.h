#ifndef K_NET_HTTP_HTTPENUMS_H
#define K_NET_HTTP_HTTPENUMS_H

namespace K {

	/** supported HTTP versions */
	enum class HttpVersion {
		HTTP_1_0,
		HTTP_1_1,
		HTTP_2
	};

	enum class HttpConnectionMode {
		UNSPECIFIED,
		CLOSE,
		KEEP_ALIVE
	};

}

#endif // K_NET_HTTP_HTTPENUMS_H
