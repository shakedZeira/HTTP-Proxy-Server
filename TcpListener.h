
#pragma once

#include <WS2tcpip.h>
#include <string>


#pragma comment (lib, "ws2_32.lib")

class TcpListener
{
public:
	TcpListener(const char* ipaddress, int portNum)
	{
		_IpAddress = ipaddress;
		_port = portNum;
	}

	int init();
	int run();

protected:

	virtual void onClientConnected(int clientSocket);

	virtual void onClientDisconnected(int clientSocket);

	virtual void onMessageRecived(int clientSocket, const char* msg, int length);

	void sendtoClient(int clientsocket, const char* msg, int length);

	void broadcastToClients(int sendingClient, const char* msg, int length);

	void GetRequest(int clientSocket , std::string File);

private:

	const char* _IpAddress;
	int _port;
	int _socket;
	fd_set _master;
};
