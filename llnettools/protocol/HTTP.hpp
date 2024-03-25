/*
 * HTTP.hpp
 *
 *  Created on: Mar 16, 2023
 *      Author: llanyro
 */

#ifndef LLCPP_NET_PROTOCOL_HTTP_HPP_
#define LLCPP_NET_PROTOCOL_HTTP_HPP_

#include "../tcp/TCPSocket.hpp"

namespace llcpp {
namespace net {
namespace http {

struct Request {
	ll_string_t method = LL_NULLPTR;
	ui8 methodLen = 0;
	ll_string_t resource = LL_NULLPTR;
	ui8 resourceLen = 0;

	ll_string_t content = LL_NULLPTR;
	len_t contentLen = 0;
};


namespace client {

class HTTP : protected tcp::ClientSocket {
	protected:
		len_t ipLen;
	private:
		void move(HTTP&& other);
	protected:
		void sendRequest(Request* request) const;
	public:
		HTTP(ll_string_t ip, const ui16 port);
		HTTP(const HTTP& other) = delete;
		HTTP(HTTP&& other);
		HTTP& operator=(const HTTP& other) = delete;
		HTTP& operator=(HTTP&& other) noexcept;
		virtual ~HTTP();

		void get() const;
		void post() const;
		void options() const;
};

} /* namespace client */

namespace server {

/*class HTTP : public tcp::ServerSocket {
	public:
		HTTP(const ui16 port);
		HTTP(HTTP& other);
		HTTP(HTTP&& other);
		HTTP& operator=(HTTP& other) noexcept;
		HTTP& operator=(HTTP&& other) noexcept;
		virtual ~HTTP();

		i32 bindPort() const;
		i32 startListening() const;
		TCPSocketBase acceptClient() const;
};*/

} /* namespace server */

using ServerSocket = llcpp::net::tcp::server::Socket;
using ClientSocket = llcpp::net::tcp::client::Socket;

} /* namespace tcp */
} /* namespace net */
} /* namespace llcpp */

#endif /* LLCPP_NET_HTTP_HPP_ */
