#ifndef K_SOCKETS_SOCKETUDP_H
#define K_SOCKETS_SOCKETUDP_H

#if defined(__GNUC__)
	#include "SocketUDP_linux.h"
#elif defined(_WIN32)
	#include "SocketUDP_win.h"
#else
	#error "TODO"
#endif

#endif // K_SOCKETS_UDPSOCKET_H
