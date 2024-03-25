/*
 * WSAController.hpp
 *
 *	Author: Francisco Julio Ruiz Fernandez
 *	Author: llanyro
 *
 *	Version: 1.0
 */

#if defined(LLANYLIB_NETTOOLS_WSACONTROLLER_HPP_) // Guard && version protector
	#if LLANYLIB_NETTOOLS_WSACONTROLLER_MAYOR_ != 1 || LLANYLIB_NETTOOLS_WSACONTROLLER_MINOR_ < 0
		#error "WSAController.hpp version error!"
	#endif // LLANYLIB_NETTOOLS_WSACONTROLLER_MAYOR_ || LLANYLIB_NETTOOLS_WSACONTROLLER_MINOR_

#else !defined(LLANYLIB_NETTOOLS_WSACONTROLLER_HPP_)
#define LLANYLIB_NETTOOLS_WSACONTROLLER_HPP_
#define LLANYLIB_NETTOOLS_WSACONTROLLER_MAYOR_ 1
#define LLANYLIB_NETTOOLS_WSACONTROLLER_MINOR_ 0

#include <llanylib/types.hpp>
#include <llanylib/Singleton.hpp>

#if !defined(WINDOWS_SYSTEM)
	#error "This module should be included only in windows systems"
#endif // WINDOWS_SYSTEM

struct WSAData;

namespace llcpp {
namespace net {

class WSAController : public singleton::static_::Singleton<WSAController> {
	private:
		WSAData* wsaData;
	private:
		#pragma region Singleton
		friend class Singleton<WSAController>;
		WSAController();
		~WSAController();
		#pragma endregion
	public:
		WSAController(const WSAController& other) __LL_EXCEPT__ = delete;
		WSAController& operator=(const WSAController& other) __LL_EXCEPT__ = delete;
		WSAController(WSAController&& other) __LL_EXCEPT__ = delete;
		WSAController& operator=(WSAController&& other) __LL_EXCEPT__ = delete;
};

} // namespace net
} // namespace llcpp

#endif // LLANYLIB_NETTOOLS_WSACONTROLLER_HPP_
