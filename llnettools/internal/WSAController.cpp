/*
 * WSAController.cpp
 *
 *	Author: Francisco Julio Ruiz Fernandez
 *	Author: llanyro
 *
 *	Version: 1.0
 */

#include "WSAController.hpp"

#include "llnetinternallib.hpp"

namespace llcpp {
namespace net {

WSAController::WSAController()
	: Singleton<WSAController>()
	, wsaData(new WSADATA())
	, wsaStatus(WSAStartup(MAKEWORD(2, 2), wsaData))
{}
WSAController::~WSAController() {
	WSACleanup();
	delete this->wsaData;
}

ll_bool_t WSAController::isOk() const __LL_EXCEPT__ {
	return this->wsaStatus == 0;
}
i32 WSAController::getStatus() const __LL_EXCEPT__ {
	return this->wsaStatus;
}

} // namespace net
} // namespace llcpp
