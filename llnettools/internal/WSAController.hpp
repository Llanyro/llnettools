/*
 *	WSAController.hpp
 *
 *	Author: Francisco Julio Ruiz Fernandez
 *	Author: llanyro
 *
 *	Version: 1.1
 */

#if defined(LLNETTOOLS_WSACONTROLLER_HPP_) // Guard && version protector
	#if LLNETTOOLS_WSACONTROLLER_MAYOR_ != 1 || LLNETTOOLS_WSACONTROLLER_MINOR_ < 1
		#error "WSAController.hpp version error!"
	#endif // LLNETTOOLS_WSACONTROLLER_MAYOR_ || LLNETTOOLS_WSACONTROLLER_MINOR_

#else !defined(LLNETTOOLS_WSACONTROLLER_HPP_)
#define LLNETTOOLS_WSACONTROLLER_HPP_
#define LLNETTOOLS_WSACONTROLLER_MAYOR_ 1
#define LLNETTOOLS_WSACONTROLLER_MINOR_ 1

#include <llanylib/types.hpp>
#include <llanylib/Singleton.hpp>

#if !defined(WINDOWS_SYSTEM)
	#error "This module should be included only in windows systems"
#endif // WINDOWS_SYSTEM

struct WSAData;

namespace llcpp {
namespace net {

class LL_SHARED_LIB WSAController : public singleton::static_::Singleton<WSAController> {
	private:
		WSAData* wsaData;
		i32 wsaStatus;
	private:
		#pragma region Singleton
		friend class singleton::static_::Singleton<WSAController>;
		WSAController();
		~WSAController();
		#pragma endregion
	public:
		WSAController(const WSAController&) __LL_EXCEPT__ = delete;
		WSAController& operator=(const WSAController&) __LL_EXCEPT__ = delete;

		WSAController(WSAController&&) __LL_EXCEPT__ = delete;
		WSAController& operator=(WSAController&&) __LL_EXCEPT__ = delete;

		__LL_NODISCARD__ ll_bool_t isOk() const __LL_EXCEPT__;
		__LL_NODISCARD__ i32 getStatus() const __LL_EXCEPT__;
};

} // namespace net
} // namespace llcpp

#endif // LLNETTOOLS_WSACONTROLLER_HPP_
