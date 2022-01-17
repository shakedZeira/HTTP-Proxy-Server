#pragma once
#include "TcpListener.h"

class WebServer : public TcpListener
{
public:

	WebServer(const char* IpAddress, int port) :
		TcpListener(IpAddress, port) {}

protected:

	virtual void onClientConnected(int clientSocket);

	virtual void onClientDisconnected(int clientSocket);

	virtual void onMessageRecived(int clientSocket, const char* msg, int length);

};