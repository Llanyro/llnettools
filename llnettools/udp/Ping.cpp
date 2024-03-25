/*
 * Ping.hpp
 *
 *  Created on: Jul 8, 2023
 *      Author: llanyro
 */

#include "Ping.hpp"

#include "../internal/llnetinternallib.hpp"

#include "../internal/WSAController.hpp"

#include <utility>

#if defined(WINDOWS_SYSTEM)
    #pragma warning(push)
    #pragma warning(disable:4820) // ignore a warning of winsock (not my problem

    #include <iphlpapi.h>
    #include <icmpapi.h>
    #pragma comment(lib, "iphlpapi.lib")
	#pragma warning(pop)
    #pragma warning(disable:4996) // insecure function
#endif // WINDOWS_SYSTEM

namespace llcpp {
namespace net {
namespace udp {

/*
*   Internal const expresions of Ping 
*/
constexpr ll_string_t ICMP_ECHO_REQUEST = "ICMP Echo Request";
constexpr b64 PACKET_SIZE = 32;
constexpr b64 REPLY_SIZE = sizeof(ICMP_ECHO_REPLY) + PACKET_SIZE + 8;


void Ping::move(Ping&& other) __LL_EXCEPT__ {
    this->ip = other.ip;
    this->icmp_handle = other.icmp_handle;
    // reply buffer no need to move

    other.ip = LL_NULLPTR;
    other.icmp_handle = LL_NULLPTR;
    if (other.reply_buffer) free(other.reply_buffer);
}

Ping::Ping() __LL_EXCEPT__
    : ip(LL_NULLPTR)
    , icmp_handle(LL_NULLPTR)
    , reply_buffer(calloc(REPLY_SIZE, 1))
{}
Ping::Ping(ll_string_t ip) __LL_EXCEPT__ : Ping() {
    this->ip = ip;
    this->icmp_handle = IcmpCreateFile();

    WSAController::getInstance();
}

Ping::Ping(Ping&& other) __LL_EXCEPT__ : Ping() { this->move(std::move(other)); }
Ping& Ping::operator=(Ping&& other) __LL_EXCEPT__ { this->move(std::move(other)); return *this; }
Ping::~Ping() __LL_EXCEPT__ {
    if(this->icmp_handle) IcmpCloseHandle(this->icmp_handle);
    if(this->reply_buffer) free(this->reply_buffer);

    this->icmp_handle = INVALID_HANDLE_VALUE;
    this->reply_buffer = LL_NULLPTR;
    this->ip = LL_NULLPTR;
}

ll_bool_t Ping::isValidSocket() const __LL_EXCEPT__ { return this->icmp_handle != INVALID_HANDLE_VALUE; }
b64 Ping::replySize() const __LL_EXCEPT__ { return REPLY_SIZE; }

PingResult Ping::ping(const ui64 timeout) __LL_EXCEPT__ {
    PingResult result = {};

    if (IcmpSendEcho(
        this->icmp_handle, inet_addr(this->ip), const_cast<ll_char_t*>(ICMP_ECHO_REQUEST),
        PACKET_SIZE, NULL, this->reply_buffer, REPLY_SIZE, timeout) == 0) {
        result.error = GetLastError();
        return result;
    }
    
    // If everyting went ok
    PICMP_ECHO_REPLY reply = reinterpret_cast<PICMP_ECHO_REPLY>(reply_buffer);
    if (reply->Status == IP_SUCCESS) {
        result.dataSize = reply->DataSize;
        result.time = reply->RoundTripTime;
        result.ttl = reply->Options.Ttl;
    }
    else 
        result.error = reply->Status;

    return result;
}

} // namespace udp
} // namespace net
} // namespace llcpp
