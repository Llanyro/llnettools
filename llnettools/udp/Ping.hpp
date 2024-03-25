/*
* 
*	************ WIP ************
* 
 * Ping.hpp
 *
 *  Created on: Jul 8, 2023
 *      Author: llanyro
 */

#ifndef LLCPP_NET_UDP_PING_HPP_
#define LLCPP_NET_UDP_PING_HPP_

#include <llanylib/definitions.hpp>
#include "../internal/Socket.hpp"

namespace llcpp {
namespace net {
namespace udp {

struct PingResult {
	ui64 time = 0ull;
	ui64 error = 0ull;
	ui16 dataSize = 0;
	ui16 ttl = 0;
};

#if defined(WINDOWS_SYSTEM)
/*
*	In windows systems Ping is implemented in a diferent way than linux
*/
class LL_SHARED_LIB Ping {
	protected:
		ll_string_t ip;
		void* icmp_handle;
		void* reply_buffer;
	protected:
		void move(Ping&& other) __LL_EXCEPT__;
	public:
		Ping() __LL_EXCEPT__;
		Ping(ll_string_t ip) __LL_EXCEPT__;
		Ping(const Ping& other) __LL_EXCEPT__ = delete;
		Ping(Ping&& other) __LL_EXCEPT__;
		Ping& operator=(const Ping& other) __LL_EXCEPT__ = delete;
		Ping& operator=(Ping&& other) __LL_EXCEPT__;
		~Ping() __LL_EXCEPT__;

		/*
		*	Return true if Windows HANDLE is in good status
		*/
		ll_bool_t isValidSocket() const __LL_EXCEPT__;
		b64 replySize() const __LL_EXCEPT__;

		PingResult ping(const ui64 timeout = 5000) __LL_EXCEPT__;
};
#else
class LL_SHARED_LIB Ping : public __internal__::Socket {
	protected:
		ll_string_t ip;
	public:
		Ping(ll_string_t ip);
		Ping(Ping& other);
		Ping& operator=(Ping&& other) noexcept;
		virtual ~Ping();

		PingResult ping();
};
#endif

} // namespace udp
} // namespace net
} // namespace llcpp

#endif /* LLCPP_NET_PING_HPP_ */
