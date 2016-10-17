// ClientDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <winsock2.h>
#include <ctime>  

using namespace std;
#pragma comment (lib, "Ws2_32.lib")

const int RepeatCount = 1024*1024;

bool Check(const char * src, const char * dest)
{
	return strcmp(src, "Abc123!@#") == 0;
}

inline unsigned __int64 GetTimeStampCount()
{
	__asm RDTSC
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*Ver 2.0*/
	const int MAX_PASSWORD_LEN = 16;
	char GotPassword[MAX_PASSWORD_LEN] = { 0 };
	int GotLen = 0;
	int OverAverageTimes[256] = { 0 };
	unsigned __int64 UsedTicks[256] = {0};

	char CurPassword[MAX_PASSWORD_LEN] = { 0 };
	char AvailableChar[256] = { 0 };
	int availabeCount = 0;

	int MaxTestTimes = 10 * 1024;
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
			bGot = Check(CurPassword, nullptr);
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
				unsigned __int64 curAvg = currentPassTotal / availabeCount;
				for (int idx = 0; idx < availabeCount; ++idx)
				{
					if (UsedTicks[idx] > curAvg)
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
			if (maxTime > averageTime + MaxTestTimes / 5)	// 1/2 ? 1/10 ?
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
		}
	}

	cout << "Got the password:" << CurPassword << endl;
	system("Pause");

	/* Ver 1.0
	cout << "Client Start" << endl;
	char TestArr[][16]=
	{
		"AAAAAAAAA",
		"Abbbbbbbb",
		"Abccccccc",
		"Abc111111",
		"Abc122222",
		"Abc123333",
		"Abc123!!!",
		"Abc123!@@",
		"Abc123!@#",
	};
	__int64 totalTime[16] = {0};
	int ret = 0;
	for (int j = 0; j < RepeatCount; ++j)
	{
		for (int i = 0; i < sizeof(TestArr) / (sizeof(char) * 16); ++i)
		{
			unsigned __int64 BeforeTime, AfterTime;
			BeforeTime = GetTimeStampCount();
			ret = Check(TestArr[i], nullptr);
			AfterTime = GetTimeStampCount();
			totalTime[i] += AfterTime - BeforeTime;
		}
	}

	for (int i = 0; i < 9; ++i)
	{
		cout << "use time:" << totalTime[i] << endl;
	}
	return 0;
	*/
}

