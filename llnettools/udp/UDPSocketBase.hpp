/*
 *	UDPSocket.hpp
 *
 *	Author: Francisco Julio Ruiz Fernandez
 *	Author: llanyro
 *
 *	Version: 1.1
 */

#if defined(LLNETTOOLS_UDPSOCKETBASE_HPP_) // Guard && version protector
	#if LLNETTOOLS_UDPSOCKETBASE_MAYOR_ != 1 || LLNETTOOLS_UDPSOCKETBASE_MINOR_ < 1
		#error "UDPSocket.hpp version error!"
	#endif // LLNETTOOLS_UDPSOCKETBASE_MAYOR_ || LLNETTOOLS_UDPSOCKETBASE_MINOR_

#else !defined(LLNETTOOLS_UDPSOCKETBASE_HPP_)
#define LLNETTOOLS_UDPSOCKETBASE_HPP_
#define LLNETTOOLS_UDPSOCKETBASE_MAYOR_ 1
#define LLNETTOOLS_UDPSOCKETBASE_MINOR_ 1

#include <llanylib/definitions.hpp>
#include "../internal/Socket.hpp"

namespace llcpp {
namespace net {
namespace udp {

class LL_SHARED_LIB UDPSocketBase : public Socket {
    public:
		UDPSocketBase(const ui16 port) __LL_EXCEPT__;
		/*
		*	Default constructor to connection opened socket
		*	Do not generate an WSData, so it doesnt open a port
		*	Just let use this socket to to read/write bytes
		*/
		UDPSocketBase(const ll_socket_t sock, sockaddr_in* addr) __LL_EXCEPT__;
		~UDPSocketBase() __LL_EXCEPT__;

		UDPSocketBase(const UDPSocketBase&) __LL_EXCEPT__ = delete;
		UDPSocketBase& operator=(const UDPSocketBase&) __LL_EXCEPT__ = delete;

		UDPSocketBase(UDPSocketBase&& other) __LL_EXCEPT__;
		UDPSocketBase& operator=(UDPSocketBase&& other) __LL_EXCEPT__;

		__LL_NODISCARD__ ui16 getPort() const __LL_EXCEPT__;
};

} // namespace udp
} // namespace net
} // namespace llcpp

#endif // LLNETTOOLS_UDPSOCKETBASE_HPP_
