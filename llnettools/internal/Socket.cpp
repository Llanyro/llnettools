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

#include <chrono>
#include <thread>

#if defined(WINDOWS_SYSTEM)
	#include "WSAController.hpp"
#endif

namespace llcpp {
namespace net {

constexpr ui64 TIMEOUT_DELAY = 1000000;

void Socket::simpleClear() __LL_EXCEPT__ {
	this->sock = INVALID_SOCKET;
	this->addr = LL_NULLPTR;
}
void Socket::initSocket(const ProtocolType protocol_type, const NetType net_type) __LL_EXCEPT__ {
	#if defined(WINDOWS_SYSTEM)
	// This inits WSA data and loads dll required
	// If there is an error WSA contains it
	// Also, this socket will stay as invalid
	if(!WSAController::getInstance().isOk()) return;
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

i32 Socket::writeBytes(const void* bytes, const ui64 length) const __LL_EXCEPT__ {
	return send(this->sock, reinterpret_cast<ll_string_t>(bytes), length, 0);
}
i32 Socket::sendBytes(const void* bytes, const ui64 length) const __LL_EXCEPT__ {
	return this->writeBytes(bytes, length);
}
i32 Socket::readBytes(void* bytes, const ui64 length) const __LL_EXCEPT__ {
	return recv(this->sock, reinterpret_cast<ll_char_t*>(bytes), length, 0);
}
Socket::IOStatus Socket::readBytes(void* bytes, const ui64 length, const ui64 timeout) const __LL_EXCEPT__ {
#if defined(WINDOWS_SYSTEM)
    u_long mode = 1; // 1 to enable non-blocking mode
    if (ioctlsocket(this->sock, FIONBIO, &mode) != 0)
        return IOStatus::ErrorCannotStartNonBlockingMode;
#elif defined(POSIX_SYSTEM) || defined(UNIX_SYSTEM)
	int flags = fcntl(this->sock, F_GETFL, 0);
	fcntl(this->sock, F_SETFL, flags | O_NONBLOCK);
#endif

	auto startTime = std::chrono::steady_clock::now();
	ui64 bytesReaded{};
	char* buffer = reinterpret_cast<char*>(bytes);

	while (bytesReaded < length) {
		auto currentTime = std::chrono::steady_clock::now();
		if (std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - startTime).count() >= timeout)
			return IOStatus::TimeOut;

		i64 bytesRead = recv(this->sock, buffer + bytesReaded, length - bytesReaded, 0);

		// Add bytes to counter
		if (bytesRead > 0) bytesReaded += bytesRead;
		// If nothing was readed, we wait to next round
		else if (bytesRead == 0)
			std::this_thread::sleep_for(std::chrono::nanoseconds(TIMEOUT_DELAY));
		else {
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				std::this_thread::sleep_for(std::chrono::nanoseconds(TIMEOUT_DELAY));
			else return IOStatus::InternalError;
		}
	}

	return IOStatus::Ok;
}


ll_bool_t Socket::isValidSocket() const __LL_EXCEPT__ { return IS_INVALID_SOCKET(this->sock); }
ll_bool_t Socket::hasError() const __LL_EXCEPT__ {
	i32 error_code{};
	i32 error_code_size = sizeof(error_code);
	getsockopt(
		this->sock,
		SOL_SOCKET,
		SO_ERROR,
		reinterpret_cast<char*>(&error_code),
		&error_code_size
	);
	return error_code != 0;
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
