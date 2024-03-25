/*
 * HTTP.cpp
 *
 *  Created on: Mar 16, 2023
 *      Author: llanyro
 */

#include "HTTP.hpp"

#include "../../string/StringBuffer.hpp"

#include <cstring>

namespace llcpp {
namespace net {
namespace http {

namespace client {

/// GET / HTTP/1.1
/// Host: localhost:8080
/// User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko/20100101 Firefox/115.0
/// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
/// Accept-Language: es-ES,es;q=0.8,en-US;q=0.5,en;q=0.3
/// Accept-Encoding: gzip, deflate, br
/// Connection: keep-alive
/// Upgrade-Insecure-Requests: 1
/// Sec-Fetch-Dest: document
/// Sec-Fetch-Mode: navigate
/// Sec-Fetch-Site: none
/// Sec-Fetch-User: ?1

Request GET_REQUEST = { "GET ", 4, "/", 1, LL_NULLPTR, 0ull };
Request POST_REQUEST = { "POST ", 5, "/", 1, LL_NULLPTR, 0ull };
Request OPTIONS_REQUEST = { "OPTIONS ", 8, "/", 1, LL_NULLPTR, 0ull };

void HTTP::move(HTTP&& other) {
	this->ipLen = other.ipLen;
	other.ipLen = 0;
}

void HTTP::sendRequest(Request* request) const {
	llcpp::string::StringBuffer buffer(1024);

	// Method line: GET / HTTP/1.1
	buffer.add(request->method, request->methodLen);
	buffer.add(request->resource, request->resourceLen);
	buffer.add(" HTTP/1.1\r\n", 11);

	// Host line: Host: localhost:8080
	buffer.add("Host: ", 6);
	buffer.add(this->ip, this->ipLen);
	buffer.add(':');
	buffer.write(this->getPort());
	buffer.add("\r\n", 2);

	// User agent line
	buffer.add("User-Agent: Llanyro's HTTP multi-platform client\r\n", 50);

	if (request->content) {
		// Lenght line
		buffer.add("Content length: ", 16);
		buffer.write(request->contentLen);
		buffer.add("\r\n", 2);
	}

	// End header
	buffer.add("\r\n", 2);

	// Send header
	this->writeBytes(buffer.begin(), buffer.getLastPosition());
	// Send header if exist
	if (request->content)
		this->writeBytes(request->content, request->contentLen);
}

HTTP::HTTP(ll_string_t ip, const ui16 port) : ClientSocket(ip, port), ipLen(std::strlen(ip)) {}
HTTP::HTTP(HTTP&& other) : ClientSocket(std::move(other)) { this->move(std::move(other)); }
HTTP& HTTP::operator=(HTTP&& other) noexcept {
	ClientSocket::operator=(std::move(other));
	this->move(std::move(other));
	return *this;
}
HTTP::~HTTP() {}

void HTTP::get() const { this->sendRequest(&GET_REQUEST); }
void HTTP::post() const { this->sendRequest(&POST_REQUEST); }
void HTTP::options() const { this->sendRequest(&OPTIONS_REQUEST); }

} /* namespace client */

namespace server {

} /* namespace server */

using ServerSocket = llcpp::net::tcp::server::Socket;
using ClientSocket = llcpp::net::tcp::client::Socket;

} /* namespace tcp */
} /* namespace net */
} /* namespace llcpp */
