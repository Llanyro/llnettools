/*
 * Socket.hpp
 *
 *	Author: Francisco Julio Ruiz Fernandez
 *	Author: llanyro
 *
 *	Version: 1.0
 */

#if defined(LLANYLIB_NETTOOLS_SOCKET_HPP_) // Guard && version protector
	#if LLANYLIB_NETTOOLS_SOCKET_MAYOR_ != 1 || LLANYLIB_NETTOOLS_SOCKET_MINOR_ < 1
		#error "Socket.hpp version error!"
	#endif // LLANYLIB_NETTOOLS_SOCKET_MAYOR_ || LLANYLIB_NETTOOLS_SOCKET_MINOR_

#else !defined(LLANYLIB_NETTOOLS_SOCKET_HPP_)
#define LLANYLIB_NETTOOLS_SOCKET_HPP_
#define LLANYLIB_NETTOOLS_SOCKET_MAYOR_ 1
#define LLANYLIB_NETTOOLS_SOCKET_MINOR_ 1

#include "llnetlib.hpp"

namespace llcpp {
namespace net {

class LL_SHARED_LIB Socket {
	public:
		enum class IOStatus : ui8 {
			NegativeTimeDiff,
			TimeOut,
			InternalError,
			ErrorCannotStartNonBlockingMode,
			Ok,
			Unknown
		};
		enum class AddressResult : ui8 {
			Ok,
			Unknown,
			InvalidLength,
			OperationError
		};
    protected:
		ll_socket_t sock;
		sockaddr_in* addr;
	protected:
		void simpleClear() __LL_EXCEPT__;
		/*
		*	All data needs to be clean before using this
		*	Or data will be lost and not deleted or freed!
		*/
		void initSocket(const ProtocolType protocol_type, const NetType net_type) __LL_EXCEPT__;
    public:
		Socket() __LL_EXCEPT__;
		/*
		*	Default constructor to connection opened socket
		*	Do not generate an WSData, so it doesnt open a port
		*	Just let use this socket to to read/write bytes
		*/
		Socket(const ll_socket_t sock, sockaddr_in* addr) __LL_EXCEPT__;
		// Creates a socket by given data
		Socket(const ProtocolType protocol_type, const NetType net_type, sockaddr_in* addr) __LL_EXCEPT__;
		// Closes socket if opened
		~Socket() __LL_EXCEPT__;

		Socket(const Socket&) __LL_EXCEPT__ = delete;
		Socket& operator=(const Socket&) __LL_EXCEPT__ = delete;

		/*
		*	Moves other socket to this socket data
		*	Other socket data will be cleaned, so this socket will be other socket
		*/
		Socket(Socket&& other) __LL_EXCEPT__;
		// Same as move contructor
		Socket& operator=(Socket&& other) __LL_EXCEPT__;

		/*
		*	In unix this could lead to a signal
		*	So i recommend to get the signal to let the program contnue normally
		*/
		__LL_NODISCARD__ i64 writeBytes(const void* data, const i64 bytes) const __LL_EXCEPT__;
		/*
		*	In unix this could lead to a signal
		*	So i recommend to get the signal to let the program contnue normally
		*/
		__LL_NODISCARD__ i32 writeBytes(const void* data, const i32 bytes) const __LL_EXCEPT__;
		// Proxy of writeBytes()
		__LL_NODISCARD__ i64 sendBytes(const void* data, const i64 bytes) const __LL_EXCEPT__;
		// Proxy of writeBytes()
		__LL_NODISCARD__ i32 sendBytes(const void* data, const i32 bytes) const __LL_EXCEPT__;

		// Reads bytes and writes into a buffer
		__LL_NODISCARD__ i64 readBytes(void* data, const i64 bytes) const __LL_EXCEPT__;
		// Reads bytes and writes into a buffer
		__LL_NODISCARD__ i32 readBytes(void* data, const i32 bytes) const __LL_EXCEPT__;
		// Like read bytes but with a timeout in nanoseconds
		// If error unblocking socket, ask WSAController (only in Windows) 
		// If error reading from socket use strerror(errno) to get error
		__LL_NODISCARD__ IOStatus readBytes(void* data, const ui64 bytes, const ui64 timeout) const __LL_EXCEPT__;

		/*
		*	Return true if socket is in good status
		*/
		__LL_NODISCARD__ ll_bool_t isValidSocket() const __LL_EXCEPT__;
		// Checks is opened socket has an error
		// Do not mistake with isValidSocket()
		__LL_NODISCARD__ ll_bool_t hasError() const __LL_EXCEPT__;

		// this can throw while creating a std::string
		__LL_NODISCARD__ ll_bool_t getAddress(std::string& str) const noexcept(false);
		__LL_NODISCARD__ AddressResult getAddress(ll_char_t* str, const len_t length) const __LL_EXCEPT__;
		__LL_NODISCARD__ AddressResult getAddress(ll_char_t (&str)[INET_ADDRSTRLENGHT]) const __LL_EXCEPT__;

		// Clears socket and makes it invalid to use
		// To use this socket again use: reset()
		void closeSocket() __LL_EXCEPT__;
		// Proxy of closeSocket()
		void clear() __LL_EXCEPT__;
		// Use this to reset or start socket
		// This uses the function clear() by itself
		void reset(const ProtocolType protocol_type, const NetType net_type) __LL_EXCEPT__;
};

} // namespace net
} // namespace llcpp

#endif // LLANYLIB_NETTOOLS_SOCKET_HPP_
