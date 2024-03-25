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

TCPSocketBase::TCPSocketBase(const ui16 port) : Socket(ProtocolType::INET, NetType::STREAM, LL_NULLPTR) {
	//this->addr->sin_family = magic_enum::enum_integer(ProtocolType::INET);
	this->addr->sin_addr.s_addr = INADDR_ANY;
	this->addr->sin_port = htons(port);
}
TCPSocketBase::TCPSocketBase(const ll_socket_t sock, sockaddr_in* addr) : Socket(sock, addr) {}
TCPSocketBase::TCPSocketBase(TCPSocketBase&& other) : Socket(std::move(other)) {}
TCPSocketBase& TCPSocketBase::operator=(TCPSocketBase&& other) noexcept {
	Socket::operator=(std::move(other));
	return *this;
}
TCPSocketBase::~TCPSocketBase() {}

ui16 TCPSocketBase::getPort() const { return ntohs(this->addr->sin_port); }

} // namespace tcp
} // namespace net
} // namespace llcpp
