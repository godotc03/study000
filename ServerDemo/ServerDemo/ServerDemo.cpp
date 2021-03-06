// SeverDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <winsock2.h>

using namespace std;
#pragma comment (lib, "Ws2_32.lib")

#define PORT 1304
#define IP_ADDRESS "127.0.0.1" 


bool Check(const char * src, const char * dest)
{
	return strcmp(src, "Abc123!@#") == 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	const int MAX_PASSWORD_LEN = 16;
	WSADATA wsa;
	SOCKET serverSocket, clientSocket;
	struct sockaddr_in LocalAddr, clientAddr;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		cout << "Init socket failed!" << endl;
		return -1;
	}

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		cout << "create socekt error" << endl;
		return -1;
	}

	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_addr.s_addr = INADDR_ANY;
	LocalAddr.sin_port = htons(PORT);
	memset(LocalAddr.sin_zero, 0, 8);

	//bind
	int ret = bind(serverSocket, (sockaddr*)&LocalAddr, sizeof(LocalAddr));
	if (ret != 0)
	{
		cout << "bind failed!" << endl;
		return -1;
	}

	ret = listen(serverSocket, 5);
	if (ret != 0)
	{
		cout << "listen failed !" << endl;
		return -1;
	}

	cout << "Sever Start" << endl;

	while (true)
	{
		int AddrLen = sizeof(clientAddr);
		clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &AddrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			cout << "accept failed" << endl;
			break;
		}

		char RecvBuf[256] = { 0 };
		char SendBuf[16] = { 0 };
		while (true)
		{
			ret = recv(clientSocket, RecvBuf, 256, 0);
			if (ret == 0 || ret == SOCKET_ERROR)
			{
				cout << "receive failed!" << endl;
				break;
			}

			ret = Check(RecvBuf, nullptr);
			memcpy(SendBuf, &ret, sizeof(ret));

			ret = send(clientSocket, SendBuf, 4, 0);

			if (ret == SOCKET_ERROR)
			{
				cout << "send error" << endl;
				break;
			}
		}

		closesocket(clientSocket);
	}

	WSACleanup();
	return 0;
}

