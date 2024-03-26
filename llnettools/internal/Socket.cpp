/*
 * Socket.hpp
 *
 *	Author: Francisco Julio Ruiz Fernandez
 *	Author: llanyro
 *
 *	Version: 1.0
 */

#include "Socket.hpp"

#include "llnetinternallib.hpp"

#include <magic_enum/magic_enum.hpp>

#if defined(WINDOWS_SYSTEM)
	#include "WSAController.hpp"
#endif

namespace llcpp {
namespace net {

void Socket::simpleClear() __LL_EXCEPT__ {
	this->sock = INVALID_SOCKET;
	this->addr = LL_NULLPTR;
}
void Socket::initSocket(const ProtocolType protocol_type, const NetType net_type) __LL_EXCEPT__ {
	#if defined(WINDOWS_SYSTEM)
	WSAController::getInstance();	// This inits wsa data and loads dll required
	#endif

	if(!this->addr) this->addr = new sockaddr_in();
	this->addr->sin_family = magic_enum::enum_integer(protocol_type);
	// Open a port
	this->sock = socket(
		magic_enum::enum_integer(protocol_type),
		magic_enum::enum_integer(net_type),
		0
	);
}


Socket::Socket() __LL_EXCEPT__ : Socket(INVALID_SOCKET, LL_NULLPTR) {}
Socket::Socket(const ll_socket_t sock, sockaddr_in* addr) __LL_EXCEPT__ : sock(sock), addr(addr) {}
Socket::Socket(const ProtocolType protocol_type, const NetType net_type, sockaddr_in* addr) __LL_EXCEPT__
	: Socket(INVALID_SOCKET, addr) {
	this->initSocket(protocol_type, net_type);
}
Socket::~Socket() __LL_EXCEPT__ { this->closeSocket(); }


Socket::Socket(Socket&& other) __LL_EXCEPT__ : Socket(other.sock, other.addr) { other.simpleClear(); }
Socket& Socket::operator=(Socket&& other) __LL_EXCEPT__ {
	this->closeSocket();
	this->sock = other.sock;
	this->addr = other.addr;
	other.simpleClear();
	return *this;
}


ll_bool_t Socket::isValidSocket() const __LL_EXCEPT__ { return IS_INVALID_SOCKET(this->sock); }

i32 Socket::writeBytes(const void* bytes, const len_t length) const __LL_EXCEPT__ {
	return send(this->sock, reinterpret_cast<ll_string_t>(bytes), length, 0);
}
i32 Socket::sendBytes(const void* bytes, const len_t length) const __LL_EXCEPT__ {
	return this->writeBytes(bytes, length);
}
i32 Socket::readBytes(void* bytes, const len_t bytesToRead) const __LL_EXCEPT__ {
	return recv(this->sock, reinterpret_cast<ll_char_t*>(bytes), bytesToRead, 0);
}

void Socket::closeSocket() __LL_EXCEPT__ {
	if (this->isValidSocket()) {
		CLOSE_SOCKET(this->sock);
		this->sock = INVALID_SOCKET;
	}
	if (this->addr != LL_NULLPTR) {
		delete this->addr;
		this->addr = LL_NULLPTR;
	}
}
void Socket::clear() __LL_EXCEPT__ { this->closeSocket(); }
void Socket::reset(const ProtocolType protocol_type, const NetType net_type) __LL_EXCEPT__ {
	this->clear();
	this->initSocket(protocol_type, net_type);
}

} // namespace net
} // namespace llcpp
