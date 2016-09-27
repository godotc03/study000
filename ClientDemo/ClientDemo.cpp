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

	int ret = 0;
	for (int j = 0; j < RepeatCount; ++j)
	{
		for (int i = 0; i < sizeof(TestArr) / (sizeof(char) * 16); ++i)
		{
			unsigned __int64 BeforeTime, AfterTime;
			//QueryPerformanceCounter(&BeforeTime);
			BeforeTime = GetTimeStampCount();
			//for (int j = 0; j < RepeatCount; ++j)
				//strcmp(TestArr[i], "Abc123!@#");
			ret = Check(TestArr[i], nullptr);
			AfterTime = GetTimeStampCount();

		}
	}

	for (int i = 0; i < 9; ++i)
	{
		cout << "use time:" << time[i] << endl;
	}
	return 0;
}

