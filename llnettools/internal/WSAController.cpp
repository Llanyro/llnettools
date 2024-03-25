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

WSAController::WSAController() {
	this->wsaData = new WSADATA();
	WSAStartup(MAKEWORD(2, 2), this->wsaData);	// Request os to dll of sockets
}
WSAController::~WSAController() { delete this->wsaData; }

} // namespace net
} // namespace llcpp
