#include "TcpServer.h"

int __cdecl main(void)
{
	TcpServer	server;

	if (!server.WSAStart())
		return -1;

	if (!server.ResolveAddrInfo())
		return -1;

	if (!server.AcceptClient())
		return -1;

	if (!server.ReceiveData())
		return -1;

	return 0;
}