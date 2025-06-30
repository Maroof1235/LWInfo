#pragma warning(push, 0)
#pragma warning (disable : 4668)
#include <windows.h>
#pragma warning(pop)

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

//GetSystemInfo(DWORD.dwProcessorType);

bool GetCpuInfo(struct SYSTEM_INFO* SysInfo)
{

	GetSystemInfo(SysInfo);

	FILETIME IdleTime, KernelTime, UserTime;

	SYSTEMTIME ConvertedIdleTime, ConvertedKernelTime, ConvertedUserTime;

	GetSystemTimes(&IdleTime, &KernelTime, &UserTime);

	FileTimeToSystemTime(&IdleTime, &ConvertedIdleTime);
	FileTimeToSystemTime(&KernelTime, &ConvertedKernelTime);
	FileTimeToSystemTime(&UserTime, &ConvertedUserTime);

	printf("Idle time: %u\n", ConvertedIdleTime.wSecond);
	printf("Kernel time: %u\n", ConvertedKernelTime.wSecond);
	printf("User time: %u\n", ConvertedUserTime.wSecond);

	//uint64_t IdleTimeCopy = ConvertedIdleTime;



	return true;
}
