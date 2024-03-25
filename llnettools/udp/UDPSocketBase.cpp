/*
 * UDPSocketBase.cpp
 *
 *	Author: Francisco Julio Ruiz Fernandez
 *	Author: llanyro
 *
 *	Version: 1.0
 */

#include "UDPSocketBase.hpp"

#include "../internal/llnetinternallib.hpp"

#include <utility>

namespace llcpp {
namespace net {
namespace udp {

UDPSocketBase::UDPSocketBase(const ui16 port) : Socket(ProtocolType::INET, NetType::DGRAM, LL_NULLPTR) {
	//this->addr->sin_family = AF_INET;
	this->addr->sin_addr.s_addr = INADDR_ANY;
	this->addr->sin_port = htons(port);
}
UDPSocketBase::UDPSocketBase(const ll_socket_t sock, sockaddr_in* addr) : Socket(sock, addr) {}
UDPSocketBase::UDPSocketBase(UDPSocketBase&& other) : Socket(std::move(other)) {}
UDPSocketBase& UDPSocketBase::operator=(UDPSocketBase&& other) noexcept {
	Socket::operator=(std::move(other));
	return *this;
}
UDPSocketBase::~UDPSocketBase() {}

ui16 UDPSocketBase::getPort() const { return ntohs(this->addr->sin_port); }

} // namespace udp
} // namespace net
} // namespace llcpp
