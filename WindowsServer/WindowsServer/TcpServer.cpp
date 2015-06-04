#include "TcpServer.h"


TcpServer::TcpServer()
	: _addrInfo(nullptr), _listenSocket(INVALID_SOCKET), _clientSocket(INVALID_SOCKET), _WsaInitialized(false)
{
}


TcpServer::~TcpServer()
{
	if (_listenSocket != INVALID_SOCKET)
		_CloseSocket(_listenSocket);
	if (_clientSocket != INVALID_SOCKET)
		_CloseSocket(_clientSocket);
	if (_addrInfo != nullptr)
		freeaddrinfo(_addrInfo);
	if (_WsaInitialized)
		WSACleanup();
}

void	TcpServer::_CloseSocket(SOCKET& socket)
{
	closesocket(socket);
	socket = INVALID_SOCKET;
}

bool	TcpServer::WSAStart()
{
	WSADATA wsaData;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return false;
	}
	_WsaInitialized = true;
	return true;
}

bool	TcpServer::ResolveAddrInfo()
{
	int iResult;
	struct addrinfo hints;

	if (_WsaInitialized == false)
		return false;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &_addrInfo);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		return false;
	}
	return true;
}

bool	TcpServer::AcceptClient()
{
	int iResult;

	if (_WsaInitialized == false || _addrInfo == nullptr)
		return false;

	if (_listenSocket == INVALID_SOCKET)
	{
		// Create a SOCKET for connecting to server
		_listenSocket = socket(_addrInfo->ai_family, _addrInfo->ai_socktype, _addrInfo->ai_protocol);
		if (_listenSocket == INVALID_SOCKET)
		{
			printf("socket failed with error: %ld\n", WSAGetLastError());
			return false;
		}

		// Setup the TCP listening socket
		iResult = bind(_listenSocket, _addrInfo->ai_addr, (int)_addrInfo->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			printf("bind failed with error: %d\n", WSAGetLastError());
			_CloseSocket(_listenSocket);
			return false;
		}

		freeaddrinfo(_addrInfo);
		_addrInfo = nullptr;

		iResult = listen(_listenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR)
		{
			printf("listen failed with error: %d\n", WSAGetLastError());
			_CloseSocket(_listenSocket);
			return false;
		}
	}

	// Accept a client socket
	_clientSocket = accept(_listenSocket, NULL, NULL);
	if (_clientSocket == INVALID_SOCKET)
	{
		printf("accept failed with error: %d\n", WSAGetLastError());
		_CloseSocket(_listenSocket);
		return false;
	}
	printf("client accepted");
	return true;
}

bool	TcpServer::ReceiveData()
{
	int iResult;

	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	if (_WsaInitialized == false || _clientSocket == INVALID_SOCKET)
		return false;

	// Receive until the peer shuts down the connection
	do
	{
		iResult = recv(_clientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			int	iSendResult;

			printf("Bytes received: %d\n", iResult);
			
			// Echo the buffer back to the sender
			iSendResult = send(_clientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR)
			{
				printf("send failed with error: %d\n", WSAGetLastError());
				_CloseSocket(_clientSocket);
				return false;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else 
		{
			printf("recv failed with error: %d\n", WSAGetLastError());
			_CloseSocket(_clientSocket);
			return false;
		}

	} while (iResult > 0);

	// shutdown the connection since we're done
	iResult = shutdown(_clientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		_CloseSocket(_clientSocket);
		return false;
	}
	return true;
}