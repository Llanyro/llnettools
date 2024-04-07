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

#include <llanylib/bits.hpp>

#include <magic_enum/magic_enum.hpp>

#include <chrono>
#include <thread>

#if defined(WINDOWS_SYSTEM)
	#include "WSAController.hpp"
#endif

namespace llcpp {
namespace net {

constexpr ui64 TIMEOUT_DELAY = 1000000;

template<class ConvertType, class ByteType, class Func = i32(*)(ll_socket_t, ByteType, i32, i32)>
inline i64 b64socket(ll_socket_t sock, ByteType bytes, const i64 length, Func f) __LL_EXCEPT__ {
	i32 result{};
	ConvertType __begin = reinterpret_cast<ConvertType>(bytes);
	ConvertType __bytes = __begin;
	auto __length = bits::i64Divisor::div(length);

	for (i32 i{}; i < __length.h; ++i) {
		result = f(sock, __bytes, bits::I32_MAX, 0);
		if (result != bits::I32_MAX) return __bytes - __begin;
		else __bytes += result;
	}
	if (__length.l > 0) {
		result = f(sock, __bytes, __length.l, 0);
		if (result != __length.l) return __bytes - __begin;
		else __bytes += result;
	}

	return __bytes - __begin;
}

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
	ui16 protocol = magic_enum::enum_integer(protocol_type);
	this->addr->sin_family = protocol;
	// Open a port
	this->sock = socket( protocol, magic_enum::enum_integer(net_type), 0);
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

i64 Socket::writeBytes(const void* data, const i64 bytes) const __LL_EXCEPT__ {
	return b64socket<ll_string_t>(this->sock, data, bytes, send);
}
i32 Socket::writeBytes(const void* data, const i32 bytes) const __LL_EXCEPT__ {
	return send(this->sock, reinterpret_cast<ll_string_t>(data), bytes, 0);
}
i64 Socket::sendBytes(const void* data, const i64 bytes) const __LL_EXCEPT__ {
	return this->writeBytes(data, bytes);
}
i32 Socket::sendBytes(const void* data, const i32 bytes) const __LL_EXCEPT__ {
	return this->writeBytes(data, bytes);
}
i64 Socket::readBytes(void* data, const i64 bytes) const __LL_EXCEPT__ {
	return b64socket<ll_char_t*>(this->sock, data, bytes, recv);
}
i32 Socket::readBytes(void* data, const i32 bytes) const __LL_EXCEPT__ {
	return recv(this->sock, reinterpret_cast<ll_char_t*>(data), bytes, 0);
}
Socket::IOStatus Socket::readBytes(void* data, const ui64 bytes, const ui64 timeout) const __LL_EXCEPT__ {
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
	ll_char_t* buffer = reinterpret_cast<ll_char_t*>(data);

	while (bytesReaded < bytes) {
		auto currentTime = std::chrono::steady_clock::now();
		auto diffTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
		if (diffTime < 0ll) return IOStatus::NegativeTimeDiff;
		if (static_cast<ui64>(diffTime) >= timeout) return IOStatus::TimeOut;

		ui64 __bytes = bytes - bytesReaded;
		i64 bytesRead = this->readBytes(
			buffer,
			(__bytes > llcpp::bits::I64_MAX)
				? llcpp::bits::I64_MAX
				: static_cast<i64>(__bytes)
		);

		// Add bytes to counter
		if (bytesRead > 0) {
			bytesReaded += bytesRead;
			buffer += bytesRead;
		}
		// If nothing was readed, we wait to next round
		else if (bytesRead == 0)
			std::this_thread::sleep_for(std::chrono::nanoseconds(TIMEOUT_DELAY));
		else {
			i32 err = GET_SOCKET_LAST_ERROR;
#if defined(WINDOWS_SYSTEM)
			if (err == WSAEWOULDBLOCK)
#elif defined(POSIX_SYSTEM) || defined(UNIX_SYSTEM)
			if (err == EAGAIN || err == EWOULDBLOCK)
#endif
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

ll_bool_t Socket::getAddress(std::string& str) const noexcept(false) {
	str = std::string(INET_ADDRSTRLEN, '\0');
	return inet_ntop(this->addr->sin_family, &this->addr->sin_addr, &str[0], INET_ADDRSTRLEN) != LL_NULLPTR;
}
Socket::AddressResult Socket::getAddress(ll_char_t* str, const len_t length) const __LL_EXCEPT__ {
	if (length < INET_ADDRSTRLEN) return AddressResult::InvalidLength;
	return 
		(inet_ntop(this->addr->sin_family, &this->addr->sin_addr, str, INET_ADDRSTRLEN) != LL_NULLPTR)
			? AddressResult::Ok
			: AddressResult::OperationError;
}
Socket::AddressResult Socket::getAddress(ll_char_t(&str)[INET_ADDRSTRLENGHT]) const __LL_EXCEPT__ {
	if constexpr (INET_ADDRSTRLENGHT < INET_ADDRSTRLEN) return AddressResult::InvalidLength;
	else
		return 
			(inet_ntop(this->addr->sin_family, &this->addr->sin_addr, str, INET_ADDRSTRLEN) != LL_NULLPTR)
				? AddressResult::Ok
				: AddressResult::OperationError;
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
