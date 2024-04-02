/*
 * llnetinternallib.hpp
 *
 *	Author: Francisco Julio Ruiz Fernandez
 *	Author: llanyro
 *
 *	Version: 1.0
 */

#if defined(LLANYLIB_NETTOOLS_NETINTERNALLIB_HPP_) // Guard && version protector
	#if LLANYLIB_NETTOOLS_NETINTERNALLIB_MAYOR_ != 1 || LLANYLIB_NETTOOLS_NETINTERNALLIB_MINOR_ < 1
		#error "llnetinternallib.hpp version error!"
	#endif // LLANYLIB_NETTOOLS_NETINTERNALLIB_MAYOR_ || LLANYLIB_NETTOOLS_NETINTERNALLIB_MINOR_

#else !defined(LLANYLIB_NETTOOLS_NETINTERNALLIB_HPP_)
#define LLANYLIB_NETTOOLS_NETINTERNALLIB_HPP_
#define LLANYLIB_NETTOOLS_NETINTERNALLIB_MAYOR_ 1
#define LLANYLIB_NETTOOLS_NETINTERNALLIB_MINOR_ 1

#if defined(WINDOWS_SYSTEM)
	#pragma warning(push)
	#pragma warning(disable:4820) // ignore a warning of winsock (not my problem)
	#pragma warning(disable:4668) // ignore a warning of winsock (not my problem)
	#pragma warning(disable:5039) // ignore a warning of winsock (not my problem)
	#include <winsock2.h>
	#include <Ws2tcpip.h>
	#pragma warning(pop)

	#define IS_INVALID_SOCKET(s) (s != INVALID_SOCKET)
	#define CLOSE_SOCKET(s) closesocket(s)
	#define GET_SOCKET_LAST_ERROR WSAGetLastError()
	#pragma comment(lib, "Ws2_32.lib") // #pragma comment(lib, "ws2_32.lib")
#elif defined(POSIX_SYSTEM) || defined(UNIX_SYSTEM)
	#include <unistd.h>
	#include <sys/types.h> 
	#include <sys/socket.h>
	#include <netinet/in.h>
	#define IS_INVALID_SOCKET(s) (s > INVALID_SOCKET)
	#define CLOSE_SOCKET(s) close(s)
	#define GET_SOCKET_LAST_ERROR errno
#endif // WINDOWS_SYSTEM || POSIX_SYSTEM || UNIX_SYSTEM

#endif // LLANYLIB_NETTOOLS_NETINTERNALLIB_HPP_
