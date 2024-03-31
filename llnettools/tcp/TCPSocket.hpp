/*
 * TCPSocket.hpp
 *
 *	Author: Francisco Julio Ruiz Fernandez
 *	Author: llanyro
 *
 *	Version: 1.0
 */

#if defined(LLANYLIB_NETTOOLS_TCPSOCKET_HPP_) // Guard && version protector
	#if LLANYLIB_NETTOOLS_TCPSOCKET_MAYOR_ != 1 || LLANYLIB_NETTOOLS_TCPSOCKET_MINOR_ < 0
		#error "TCPSocket.hpp version error!"
	#endif // LLANYLIB_NETTOOLS_TCPSOCKET_MAYOR_ || LLANYLIB_NETTOOLS_TCPSOCKET_MINOR_

#else !defined(LLANYLIB_NETTOOLS_TCPSOCKET_HPP_)
#define LLANYLIB_NETTOOLS_TCPSOCKET_HPP_
#define LLANYLIB_NETTOOLS_TCPSOCKET_MAYOR_ 1
#define LLANYLIB_NETTOOLS_TCPSOCKET_MINOR_ 0

#include "TCPSocketBase.hpp"

namespace llcpp {
namespace net {
namespace tcp {

namespace client {

class LL_SHARED_LIB Socket : public TCPSocketBase {
	public:
		enum class LastError {
			Ok,
			NO_VALID_SOCKET,
			IP_NULL,
			SERVER_NOT_FOUND,
			ERROR_CANT_CONNECT_TO_SERVER,
			EMPTY
		};
	protected:
		ll_string_t ip;
		LastError lastError;
	private:
		void move(Socket&& other) __LL_EXCEPT__;
	public:
		Socket(ll_string_t ip, const ui16 port) __LL_EXCEPT__;
		~Socket() __LL_EXCEPT__;

		Socket(const Socket&) __LL_EXCEPT__ = delete;
		Socket& operator=(const Socket&) __LL_EXCEPT__ = delete;

		Socket(Socket&& other) __LL_EXCEPT__;
		Socket& operator=(Socket&& other) __LL_EXCEPT__;

		__LL_NODISCARD__ LastError getLastError() const __LL_EXCEPT__;
		__LL_NODISCARD__ ll_bool_t isValidSocket() const __LL_EXCEPT__;
};

} // namespace client
namespace server {

class LL_SHARED_LIB Socket : public TCPSocketBase {
	protected:
		// Redefinition to not be used by user
		__LL_NODISCARD__ i64 writeBytes(const void* data, const i64 bytes) const __LL_EXCEPT__;
		// Redefinition to not be used by user
		__LL_NODISCARD__ i64 sendBytes(const void* data, const i64 bytes) const __LL_EXCEPT__;
		// Redefinition to not be used by user
		__LL_NODISCARD__ i64 readBytes(void* data, const i64 bytes) const __LL_EXCEPT__;

		// Redefinition to not be used by user
		__LL_NODISCARD__ i32 writeBytes(const void* data, const i32 bytes) const __LL_EXCEPT__;
		// Redefinition to not be used by user
		__LL_NODISCARD__ i32 sendBytes(const void* data, const i32 bytes) const __LL_EXCEPT__;
		// Redefinition to not be used by user
		__LL_NODISCARD__ i32 readBytes(void* data, const i32 bytes) const __LL_EXCEPT__;
	public:
		Socket(const ui16 port) __LL_EXCEPT__;
		~Socket() __LL_EXCEPT__;

		Socket(const Socket&) __LL_EXCEPT__ = delete;
		Socket& operator=(const Socket&) __LL_EXCEPT__ = delete;

		Socket(Socket&& other) __LL_EXCEPT__;
		Socket& operator=(Socket&& other) __LL_EXCEPT__;

		__LL_NODISCARD__ i32 bindPort() const __LL_EXCEPT__;
		__LL_NODISCARD__ i32 startListening() const __LL_EXCEPT__;
		__LL_NODISCARD__ TCPSocketBase acceptClient() const __LL_EXCEPT__;
};

} // namespace server

using ServerSocket = llcpp::net::tcp::server::Socket;
using ClientSocket = llcpp::net::tcp::client::Socket;

} // namespace tcp
} // namespace net
} // namespace llcpp

#endif // LLCPP_NET_TCP_TCPSOCKET_HPP_
