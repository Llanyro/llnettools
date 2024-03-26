/*
 * Socket.hpp
 *
 *	Author: Francisco Julio Ruiz Fernandez
 *	Author: llanyro
 *
 *	Version: 1.0
 */

#if defined(LLANYLIB_NETTOOLS_SOCKET_HPP_) // Guard && version protector
	#if LLANYLIB_NETTOOLS_SOCKET_MAYOR_ != 1 || LLANYLIB_NETTOOLS_SOCKET_MINOR_ < 0
		#error "Socket.hpp version error!"
	#endif // LLANYLIB_NETTOOLS_SOCKET_MAYOR_ || LLANYLIB_NETTOOLS_SOCKET_MINOR_

#else !defined(LLANYLIB_NETTOOLS_SOCKET_HPP_)
#define LLANYLIB_NETTOOLS_SOCKET_HPP_
#define LLANYLIB_NETTOOLS_SOCKET_MAYOR_ 1
#define LLANYLIB_NETTOOLS_SOCKET_MINOR_ 0

#include "llnetlib.hpp"

namespace llcpp {
namespace net {

class LL_SHARED_LIB Socket {
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
		*	Return true if socket is in good status
		*/
		__LL_NODISCARD__ ll_bool_t isValidSocket() const __LL_EXCEPT__;

		/*
		*	In unix this could lead to a signal
		*	So i recommend to get the signal to let the program contnue normally
		*/
		__LL_NODISCARD__ i32 writeBytes(const void* bytes, const len_t length) const __LL_EXCEPT__;
		// Proxy of writeBytes()
		__LL_NODISCARD__ i32 sendBytes(const void* bytes, const len_t length) const __LL_EXCEPT__;
		/*
		*	...
		*/
		__LL_NODISCARD__ i32 readBytes(void* bytes, const len_t bytesToRead) const __LL_EXCEPT__;

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
