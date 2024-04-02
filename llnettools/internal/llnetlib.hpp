/*
 * llnetlib.hpp
 *
 *	Author: Francisco Julio Ruiz Fernandez
 *	Author: llanyro
 *
 *	Version: 1.0
 */

#if defined(LLANYLIB_NETTOOLS_NETLIB_HPP_) // Guard && version protector
	#if LLANYLIB_NETTOOLS_NETLIB_MAYOR_ != 1 || LLANYLIB_NETTOOLS_NETLIB_MINOR_ < 1
		#error "llnetlib.hpp version error!"
	#endif // LLANYLIB_NETTOOLS_NETLIB_MAYOR_ || LLANYLIB_NETTOOLS_NETLIB_MINOR_

#else !defined(LLANYLIB_NETTOOLS_NETLIB_HPP_)
#define LLANYLIB_NETTOOLS_NETLIB_HPP_
#define LLANYLIB_NETTOOLS_NETLIB_MAYOR_ 1
#define LLANYLIB_NETTOOLS_NETLIB_MINOR_ 1

#include <llanylib/definitions.hpp>
#include <llanylib/types.hpp>

struct sockaddr_in;		// Basic structure in windows/linux

namespace llcpp {
namespace net {

#if defined(WINDOWS_SYSTEM)
using ll_socket_t = ui64;		// Socket descriptor
using ADDRESS_FAMILY = ui16;

#elif defined(POSIX_SYSTEM) || defined(UNIX_SYSTEM)
using ll_socket_t = i32;					// Socket descriptor
using ADDRESS_FAMILY = ui32;
constexpr ll_socket_t INVALID_SOCKET = -1;	// Value of invalid socket

#endif // WINDOWS_SYSTEM || POSIX_SYSTEM || UNIX_SYSTEM

using in_addr_t = ui32;
constexpr ui8 INET_ADDRSTRLENGHT = 22;

enum class ProtocolType : ADDRESS_FAMILY {
	UNSPEC       = 0,               // unspecified
	UNIX         = 1,               // local to host (pipes, portals)
	INET         = 2,               // internetwork: UDP, TCP, etc.
	IMPLINK      = 3,               // arpanet imp addresses
	PUP          = 4,               // pup protocols: e.g. BSP
	CHAOS        = 5,               // mit CHAOS protocols
	NS           = 6,               // XEROX NS protocols
	IPX          = NS,				// IPX protocols: IPX, SPX, etc.
	ISO          = 7,               // ISO protocols
	OSI          = ISO,             // OSI is ISO
	ECMA         = 8,               // european computer manufacturers
	DATAKIT      = 9,               // datakit protocols
	CCITT        = 10,              // CCITT protocols, X.25 etc
	SNA          = 11,              // IBM SNA
	DECnet       = 12,              // DECnet
	DLI          = 13,              // Direct data link interface
	LAT          = 14,              // LAT
	HYLINK       = 15,              // NSC Hyperchannel
	APPLETALK    = 16,              // AppleTalk
	NETBIOS      = 17,              // NetBios-style addresses
	VOICEVIEW    = 18,              // VoiceView
	FIREFOX      = 19,              // Protocols from Firefox
	UNKNOWN1     = 20,              // Somebody is using this!
	BAN          = 21,              // Banyan
	ATM          = 22,              // Native ATM Services
	INET6        = 23,              // Internetwork Version 6
	CLUSTER      = 24,              // Microsoft Wolfpack
	_12844       = 25,              // IEEE 1284.4 WG AF
	IRDA         = 26,              // IrDA
	NETDES       = 28,              // Network Designers OSI & gateway

#if defined(_WIN32_WINNT)
	#if (_WIN32_WINNT < 0x0501)
		MAX          = 29,
	#else //(_WIN32_WINNT < 0x0501)

		TCNPROCESS   = 29,
		TCNMESSAGE   = 30,
		ICLFXBM      = 31,

	# if(_WIN32_WINNT < 0x0600)
		MAX          = 32,
	# else //(_WIN32_WINNT < 0x0600)
		BTH          = 32,              // Bluetooth RFCOMM/L2CAP protocols
	#  if(_WIN32_WINNT < 0x0601)
		MAX          = 33,
	#  else //(_WIN32_WINNT < 0x0601)
		LINK         = 33,
	#   if(_WIN32_WINNT < 0x0604)
		MAX          = 34,
	#   else //(_WIN32_WINNT < 0x0604)
		HYPERV       = 34,
		MAX          = 35,
	#   endif //(_WIN32_WINNT < 0x0604)
	#  endif //(_WIN32_WINNT < 0x0601)
	# endif //(_WIN32_WINNT < 0x0600)
	#endif //(_WIN32_WINNT < 0x0501)
#endif // _WIN32_WINNT
	Unknown
};
enum class NetType {
	STREAM     = 1,               /* stream socket */
	DGRAM      = 2,               /* datagram socket */
	RAW        = 3,               /* raw-protocol interface */
	RDM        = 4,               /* reliably-delivered message */
	SEQPACKET  = 5,               /* sequenced packet stream */

	Unknown
};

} // namespace net
} // namespace llcpp

#endif // LLANYLIB_NETTOOLS_NETLIB_HPP_
