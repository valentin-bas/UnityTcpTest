#pragma once

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "1337"

class TcpServer
{
public:
	TcpServer();
	~TcpServer();

	bool	WSAStart();
	bool	ResolveAddrInfo();
	bool	AcceptClient();
	bool	ReceiveData();

private:
	struct addrinfo		*_addrInfo;
	SOCKET				_listenSocket;
	SOCKET				_clientSocket;
	bool				_WsaInitialized;

	void	_CloseSocket(SOCKET& socket);

};

