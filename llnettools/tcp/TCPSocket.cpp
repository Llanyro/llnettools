/*
 * TCPSocket.cpp
 *
 *	Author: Francisco Julio Ruiz Fernandez
 *	Author: llanyro
 *
 *	Version: 1.0
 */

#include "TCPSocket.hpp"

#include "../internal/llnetinternallib.hpp"

#include <utility>

namespace llcpp {
namespace net {
namespace tcp {

namespace client {

void Socket::move(Socket&& other) __LL_EXCEPT__ {
    this->ip = other.ip;
    this->lastError = other.lastError;

    other.ip = LL_NULLPTR;
    other.lastError = LastError::EMPTY;
}

Socket::Socket(ll_string_t ip, const ui16 port) __LL_EXCEPT__
    : TCPSocketBase(port), ip(ip), lastError(LastError::Ok) {

    if (!this->isValidSocket()) {
        this->lastError = LastError::NO_VALID_SOCKET;
        return;
    }
    else if (!ip) {
        this->lastError = LastError::IP_NULL;
        return;
    }
    else if (inet_pton(AF_INET, ip, &this->addr->sin_addr) <= 0) {
        this->lastError = LastError::SERVER_NOT_FOUND;
        return;
    }
    else if (connect(sock, (struct sockaddr*) this->addr, sizeof(*this->addr)) < 0) {
        this->lastError = LastError::ERROR_CANT_CONNECT_TO_SERVER;
        return;
    }
}
Socket::~Socket() {}

Socket::Socket(Socket&& other) __LL_EXCEPT__
    : TCPSocketBase(std::move(other)) { this->move(std::move(other)); }
Socket& Socket::operator=(Socket&& other) __LL_EXCEPT__ {
    TCPSocketBase::operator=(std::move(other));
    this->move(std::move(other));
	return *this;
}

Socket::LastError Socket::getLastError() const __LL_EXCEPT__ { return this->lastError; }
ll_bool_t Socket::isValidSocket() const __LL_EXCEPT__ {
    return TCPSocketBase::isValidSocket() && this->lastError == LastError::Ok;
}

} // namespace client
namespace server {

i64 Socket::writeBytes(const void* data, const i64 bytes) const __LL_EXCEPT__ {
    return TCPSocketBase::writeBytes(data, bytes);
}
i64 Socket::sendBytes(const void* data, const i64 bytes) const __LL_EXCEPT__ {
    return TCPSocketBase::sendBytes(data, bytes);
}
i64 Socket::readBytes(void* data, const i64 bytes) const __LL_EXCEPT__ {
    return TCPSocketBase::readBytes(data, bytes);
}

i32 Socket::writeBytes(const void* data, const i32 bytes) const __LL_EXCEPT__ {
    return TCPSocketBase::writeBytes(data, bytes);
}
i32 Socket::sendBytes(const void* data, const i32 bytes) const __LL_EXCEPT__ {
    return TCPSocketBase::sendBytes(data, bytes);
}
i32 Socket::readBytes(void* data, const i32 bytes) const __LL_EXCEPT__ {
    return TCPSocketBase::readBytes(data, bytes);
}

Socket::Socket(const ui16 port) __LL_EXCEPT__ : TCPSocketBase(port) {}
Socket::~Socket() __LL_EXCEPT__ {}

Socket::Socket(Socket&& other) __LL_EXCEPT__ : TCPSocketBase(std::move(other)) {}
Socket& Socket::operator=(Socket&& other) __LL_EXCEPT__ {
    TCPSocketBase::operator=(std::move(other));
	return *this;
}

i32 Socket::bindPort() const __LL_EXCEPT__ {
	return bind(
        this->sock,
        reinterpret_cast<struct sockaddr*>(this->addr),
        sizeof(*addr)
    );
}
i32 Socket::startListening() const __LL_EXCEPT__ { return listen(this->sock, 5); }
TCPSocketBase Socket::acceptClient() const __LL_EXCEPT__ {
	sockaddr_in* cli_addr = new sockaddr_in();
	socklen_t clilen = sizeof(*cli_addr);
	ll_socket_t newsockfd = accept(this->sock, (struct sockaddr*)cli_addr, &clilen);

	return TCPSocketBase(newsockfd, cli_addr);
}

} // namespace server

} // namespace tcp
} // namespace net
} // namespace llcpp
