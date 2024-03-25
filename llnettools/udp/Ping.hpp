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

#include "../internal/Socket.hpp"

namespace llcpp {
namespace net {
namespace udp {

struct PingResult {
	ui16 dataSize = 0;
	ui16 ttl = 0;
	ui64 time = 0ull;
	ui64 error = 0ull;
};

#if defined(WINDOWS_SYSTEM)
/*
*	In windows systems Ping is implemented in a diferent way than linux
*/
class Ping {
	protected:
		ll_string_t ip;
		void* icmp_handle;
		void* reply_buffer;
		WSAData* wsaData;
	protected:
		void move(Ping&& other);
	public:
		Ping();
		Ping(ll_string_t ip);
		Ping(const Ping& other) = delete;
		Ping(Ping&& other);
		Ping& operator=(const Ping& other) = delete;
		Ping& operator=(Ping&& other) noexcept;
		virtual ~Ping();

		/*
		*	Return true if Windows HANDLE is in good status
		*/
		virtual ll_bool_t isValidSocket() const;
		b64 replySize() const;

		PingResult ping(const ui64 timeout = 5000);
};
#else
class Ping : public __internal__::Socket {
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
