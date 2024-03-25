/*
 * Ping.hpp
 *
 *  Created on: Jul 8, 2023
 *      Author: llanyro
 */

#include "Ping.hpp"

#if defined(WINDOWS_SYSTEM)
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <icmpapi.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

#elif defined(__unix__)
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#define IS_INVALID_SOCKET(s) (s > 0)
#define CLOSE_SOCKET(s) close(s)
#endif

namespace llcpp {
namespace net {
namespace udp {

/*
*   Internal const expresions of Ping 
*/
constexpr ll_string_t ICMP_ECHO_REQUEST = "ICMP Echo Request";
constexpr b64 PACKET_SIZE = 32;
constexpr b64 REPLY_SIZE = sizeof(ICMP_ECHO_REPLY) + PACKET_SIZE + 8;


void Ping::move(Ping&& other) {
    this->ip = other.ip;
    this->icmp_handle = other.icmp_handle;
    this->wsaData = other.wsaData;
    // reply buffer no need to move

    other.ip = LL_NULLPTR;
    other.icmp_handle = LL_NULLPTR;
    other.wsaData = LL_NULLPTR;
    if (other.reply_buffer) free(other.reply_buffer);
}

Ping::Ping()
    : ip(LL_NULLPTR)
    , icmp_handle(LL_NULLPTR)
    , reply_buffer(calloc(REPLY_SIZE, 1))
    , wsaData(LL_NULLPTR)
{}
Ping::Ping(ll_string_t ip) : Ping() {
    this->ip = ip;
    this->icmp_handle = IcmpCreateFile();
    this->wsaData = new WSAData();
    WSAStartup(MAKEWORD(2, 2), this->wsaData);	// Request os to let open ports
}
Ping::Ping(Ping&& other) : Ping() { this->move(std::move(other)); }
Ping& Ping::operator=(Ping&& other) noexcept { this->move(std::move(other)); return *this; }
Ping::~Ping() {
    if(this->icmp_handle) IcmpCloseHandle(this->icmp_handle);
    if(this->reply_buffer) free(this->reply_buffer);

    this->icmp_handle = INVALID_HANDLE_VALUE;
    this->reply_buffer = LL_NULLPTR;
    this->ip = LL_NULLPTR;

    if (this->wsaData != LL_NULLPTR) {
        WSACleanup();
        delete this->wsaData;
        this->wsaData = LL_NULLPTR;
    }
}

ll_bool_t Ping::isValidSocket() const { return this->icmp_handle != INVALID_HANDLE_VALUE; }
b64 Ping::replySize() const { return REPLY_SIZE; }

PingResult Ping::ping(const ui64 timeout) {
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
