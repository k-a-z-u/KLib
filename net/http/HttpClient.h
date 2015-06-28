#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpHeader.h"

#include "../../streams/InputStream.h"
#include "../../streams/OutputStream.h"
#include "../../streams/BufferedOutputStream.h"
#include "../../streams/BufferedInputStream.h"

#include "../../sockets/SocketOutputStream.h"
#include "../../sockets/SocketInputStream.h"

#include "../../sockets/Socket.h"

namespace K {

	class HttpClient {

	private:

		Socket sck;

	public:
		struct Res {
			HttpResponse resp;
			BufferedInputStream* is;
			Res(HttpResponse& resp, BufferedInputStream* is) : resp(resp), is(is) {;}
		};

	public:

		Res request(HttpRequest& req) {

			std::string out;
			out += req.getAsString();
			out += "\r\n";
			out += req.getHeader().getAsString();
			out += "\r\n";
			std::cout << out << std::endl;

			std::string host = req.getURL().getHost();

			sck.connect(NetworkAddress(host, 80));
			sck.getOutputStream()->write((uint8_t*)out.c_str(), out.length());
			sck.getOutputStream()->flush();

			SocketInputStream* is = sck.getInputStream();
			BufferedInputStream* bis = new BufferedInputStream(is);
			LineInputStream lis(bis);

			HttpResponse resp(lis);
			return (Res(resp, bis));

		}

	};

}

#endif // HTTPCLIENT_H
