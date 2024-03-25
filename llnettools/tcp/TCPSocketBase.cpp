/*
 * TCPSocketBase.cpp
 *
 *	Author: Francisco Julio Ruiz Fernandez
 *	Author: llanyro
 *
 *	Version: 1.0
 */

#include "TCPSocketBase.hpp"

#include "../internal/llnetinternallib.hpp"

#include <utility>

namespace llcpp {
namespace net {
namespace tcp {

TCPSocketBase::TCPSocketBase(const ui16 port) __LL_EXCEPT__
	: Socket(ProtocolType::INET, NetType::STREAM, LL_NULLPTR) {
	//this->addr->sin_family = magic_enum::enum_integer(ProtocolType::INET);
	this->addr->sin_addr.s_addr = INADDR_ANY;
	this->addr->sin_port = htons(port);
}
TCPSocketBase::TCPSocketBase(const ll_socket_t sock, sockaddr_in* addr) __LL_EXCEPT__
	: Socket(sock, addr) {}

TCPSocketBase::TCPSocketBase(TCPSocketBase&& other) __LL_EXCEPT__
	: Socket(std::move(other)) {}
TCPSocketBase& TCPSocketBase::operator=(TCPSocketBase&& other) __LL_EXCEPT__ {
	Socket::operator=(std::move(other));
	return *this;
}
TCPSocketBase::~TCPSocketBase() __LL_EXCEPT__ {}

ui16 TCPSocketBase::getPort() const __LL_EXCEPT__ { return ntohs(this->addr->sin_port); }

} // namespace tcp
} // namespace net
} // namespace llcpp
