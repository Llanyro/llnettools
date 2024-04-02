/*
 *	TCPSocketBase.hpp
 *
 *	Author: Francisco Julio Ruiz Fernandez
 *	Author: llanyro
 *
 *	Version: 1.1
 */

#if defined(LLNETTOOLS_TCPSOCKETBASE_HPP_) // Guard && version protector
	#if LLNETTOOLS_TCPSOCKETBASE_MAYOR_ != 1 || LLNETTOOLS_TCPSOCKETBASE_MINOR_ < 1
		#error "TCPSocketBase.hpp version error!"
	#endif // LLNETTOOLS_TCPSOCKETBASE_MAYOR_ || LLNETTOOLS_TCPSOCKETBASE_MINOR_

#else !defined(LLNETTOOLS_TCPSOCKETBASE_HPP_)
#define LLNETTOOLS_TCPSOCKETBASE_HPP_
#define LLNETTOOLS_TCPSOCKETBASE_MAYOR_ 1
#define LLNETTOOLS_TCPSOCKETBASE_MINOR_ 1

#include <llanylib/definitions.hpp>
#include "../internal/Socket.hpp"

namespace llcpp {
namespace net {
namespace tcp {

class LL_SHARED_LIB TCPSocketBase : public Socket {
    public:
		TCPSocketBase(const ui16 port) __LL_EXCEPT__;
		/*
		*	Default constructor to connection opened socket
		*	Do not generate an WSData, so it doesnt open a port
		*	Just let use this socket to to read/write bytes
		*/
		TCPSocketBase(const ll_socket_t sock, sockaddr_in* addr) __LL_EXCEPT__;
		~TCPSocketBase() __LL_EXCEPT__;

		TCPSocketBase(const TCPSocketBase&) __LL_EXCEPT__ = delete;
		TCPSocketBase& operator=(const TCPSocketBase&) __LL_EXCEPT__ = delete;

		TCPSocketBase(TCPSocketBase&& other) __LL_EXCEPT__;
		TCPSocketBase& operator=(TCPSocketBase&& other) __LL_EXCEPT__;

		__LL_NODISCARD__ ui16 getPort() const __LL_EXCEPT__;
};

} // namespace tcp
} // namespace net
} // namespace llcpp

#endif // LLNETTOOLS_TCPSOCKETBASE_HPP_
